#include "duckdb/execution/operator/helper/physical_buffered_batch_collector.hpp"

#include "duckdb/common/types/batched_data_collection.hpp"
#include "duckdb/main/client_context.hpp"
#include "duckdb/main/materialized_query_result.hpp"
#include "duckdb/main/buffered_data/buffered_data.hpp"
#include "duckdb/main/buffered_data/batched_buffered_data.hpp"
#include "duckdb/main/stream_query_result.hpp"

namespace duckdb {

PhysicalBufferedBatchCollector::PhysicalBufferedBatchCollector(PreparedStatementData &data)
    : PhysicalResultCollector(data) {
}

//===--------------------------------------------------------------------===//
// Sink
//===--------------------------------------------------------------------===//
class BufferedBatchCollectorGlobalState : public GlobalSinkState {
public:
	mutex glock;
	shared_ptr<ClientContext> context;
	shared_ptr<BufferedData> buffered_data;
};

BufferedBatchCollectorLocalState::BufferedBatchCollectorLocalState(Pipeline &pipeline) : pipeline(pipeline) {
}

void BufferedBatchCollectorLocalState::BufferChunk(unique_ptr<DataChunk> chunk) {
	buffered_chunks.push(std::move(chunk));
}

SinkResultType PhysicalBufferedBatchCollector::Sink(ExecutionContext &context, DataChunk &chunk,
                                                    OperatorSinkInput &input) const {
	auto &gstate = input.global_state.Cast<BufferedBatchCollectorGlobalState>();
	auto &lstate = input.local_state.Cast<BufferedBatchCollectorLocalState>();

	lock_guard<mutex> l(gstate.glock);

	auto batch = lstate.BatchIndex();
	auto min_batch_index = lstate.GetMinimumBatchIndex();
	// printf("SINK: Batch: %llu, Min Batch: %llu\n", batch, min_batch_index);
	auto &buffered_data = dynamic_cast<BatchedBufferedData &>(*gstate.buffered_data);
	buffered_data.UpdateMinBatchIndex(min_batch_index);

	if (!lstate.blocked) {
		// Always block the first time
		lstate.blocked = true;
		auto callback_state = input.interrupt_state;
		auto blocked_sink = BlockedSink(callback_state, chunk.size(), batch);
		buffered_data.AddToBacklog(blocked_sink);
		return SinkResultType::BLOCKED;
	}

	if (buffered_data.ShouldBlockBatch(batch)) {
		// Block again when we've already buffered enough chunks
		auto callback_state = input.interrupt_state;
		auto blocked_sink = BlockedSink(callback_state, chunk.size(), batch);
		buffered_data.AddToBacklog(blocked_sink);
		return SinkResultType::BLOCKED;
	}

	auto to_append = make_uniq<DataChunk>();
	to_append->Initialize(Allocator::DefaultAllocator(), chunk.GetTypes());
	chunk.Copy(*to_append, 0);

	buffered_data.Append(std::move(to_append), lstate);
	return SinkResultType::NEED_MORE_INPUT;
}

SinkNextBatchType PhysicalBufferedBatchCollector::NextBatch(ExecutionContext &context,
                                                            OperatorSinkNextBatchInput &input) const {
	return SinkNextBatchType::READY;
}

SinkCombineResultType PhysicalBufferedBatchCollector::Combine(ExecutionContext &context,
                                                              OperatorSinkCombineInput &input) const {
	auto &gstate = input.global_state.Cast<BufferedBatchCollectorGlobalState>();
	auto &lstate = input.local_state.Cast<BufferedBatchCollectorLocalState>();

	lock_guard<mutex> l(gstate.glock);

	auto batch = lstate.BatchIndex();
	auto min_batch_index = lstate.GetMinimumBatchIndex();
	// printf("COMBINE: Batch: %llu, Min Batch: %llu\n", batch, min_batch_index);
	auto &buffered_data = dynamic_cast<BatchedBufferedData &>(*gstate.buffered_data);
	buffered_data.UpdateMinBatchIndex(min_batch_index);
	return SinkCombineResultType::FINISHED;
}

unique_ptr<LocalSinkState> PhysicalBufferedBatchCollector::GetLocalSinkState(ExecutionContext &context) const {
	auto state = make_uniq<BufferedBatchCollectorLocalState>(*context.pipeline);
	return std::move(state);
}

unique_ptr<GlobalSinkState> PhysicalBufferedBatchCollector::GetGlobalSinkState(ClientContext &context) const {
	auto state = make_uniq<BufferedBatchCollectorGlobalState>();
	state->context = context.shared_from_this();
	state->buffered_data = make_shared<BatchedBufferedData>(state->context);
	return std::move(state);
}

unique_ptr<QueryResult> PhysicalBufferedBatchCollector::GetResult(GlobalSinkState &state) {
	auto &gstate = state.Cast<BufferedBatchCollectorGlobalState>();
	lock_guard<mutex> l(gstate.glock);
	auto result = make_uniq<StreamQueryResult>(statement_type, properties, types, names,
	                                           gstate.context->GetClientProperties(), gstate.buffered_data);
	return std::move(result);
}

} // namespace duckdb
