//===----------------------------------------------------------------------===//
// This file is automatically generated by scripts/generate_serialization.py
// Do not edit this file manually, your changes will be overwritten
//===----------------------------------------------------------------------===//

#include "duckdb/common/serializer/serializer.hpp"
#include "duckdb/common/serializer/deserializer.hpp"
#include "duckdb/planner/expression/list.hpp"

namespace duckdb {

void Expression::Serialize(Serializer &serializer) const {
	serializer.WriteProperty(100, "expression_class", expression_class);
	serializer.WriteProperty(101, "type", type);
	serializer.WriteProperty(102, "alias", alias);
}

unique_ptr<Expression> Expression::Deserialize(Deserializer &deserializer) {
	auto expression_class = deserializer.ReadProperty<ExpressionClass>(100, "expression_class");
	auto type = deserializer.ReadProperty<ExpressionType>(101, "type");
	auto alias = deserializer.ReadProperty<string>(102, "alias");
	deserializer.Set<ExpressionType>(type);
	unique_ptr<Expression> result;
	switch (expression_class) {
	case ExpressionClass::BOUND_AGGREGATE:
		result = BoundAggregateExpression::Deserialize(deserializer);
		break;
	case ExpressionClass::BOUND_BETWEEN:
		result = BoundBetweenExpression::Deserialize(deserializer);
		break;
	case ExpressionClass::BOUND_CASE:
		result = BoundCaseExpression::Deserialize(deserializer);
		break;
	case ExpressionClass::BOUND_CAST:
		result = BoundCastExpression::Deserialize(deserializer);
		break;
	case ExpressionClass::BOUND_COLUMN_REF:
		result = BoundColumnRefExpression::Deserialize(deserializer);
		break;
	case ExpressionClass::BOUND_COMPARISON:
		result = BoundComparisonExpression::Deserialize(deserializer);
		break;
	case ExpressionClass::BOUND_CONJUNCTION:
		result = BoundConjunctionExpression::Deserialize(deserializer);
		break;
	case ExpressionClass::BOUND_CONSTANT:
		result = BoundConstantExpression::Deserialize(deserializer);
		break;
	case ExpressionClass::BOUND_DEFAULT:
		result = BoundDefaultExpression::Deserialize(deserializer);
		break;
	case ExpressionClass::BOUND_FUNCTION:
		result = BoundFunctionExpression::Deserialize(deserializer);
		break;
	case ExpressionClass::BOUND_LAMBDA:
		result = BoundLambdaExpression::Deserialize(deserializer);
		break;
	case ExpressionClass::BOUND_LAMBDA_REF:
		result = BoundLambdaRefExpression::Deserialize(deserializer);
		break;
	case ExpressionClass::BOUND_OPERATOR:
		result = BoundOperatorExpression::Deserialize(deserializer);
		break;
	case ExpressionClass::BOUND_PARAMETER:
		result = BoundParameterExpression::Deserialize(deserializer);
		break;
	case ExpressionClass::BOUND_REF:
		result = BoundReferenceExpression::Deserialize(deserializer);
		break;
	case ExpressionClass::BOUND_UNNEST:
		result = BoundUnnestExpression::Deserialize(deserializer);
		break;
	case ExpressionClass::BOUND_WINDOW:
		result = BoundWindowExpression::Deserialize(deserializer);
		break;
	default:
		throw SerializationException("Unsupported type for deserialization of Expression!");
	}
	deserializer.Unset<ExpressionType>();
	result->alias = std::move(alias);
	return result;
}

void BoundBetweenExpression::Serialize(Serializer &serializer) const {
	Expression::Serialize(serializer);
	serializer.WriteProperty(200, "input", input);
	serializer.WriteProperty(201, "lower", lower);
	serializer.WriteProperty(202, "upper", upper);
	serializer.WriteProperty(203, "lower_inclusive", lower_inclusive);
	serializer.WriteProperty(204, "upper_inclusive", upper_inclusive);
}

unique_ptr<Expression> BoundBetweenExpression::Deserialize(Deserializer &deserializer) {
	auto result = duckdb::unique_ptr<BoundBetweenExpression>(new BoundBetweenExpression());
	deserializer.ReadProperty(200, "input", result->input);
	deserializer.ReadProperty(201, "lower", result->lower);
	deserializer.ReadProperty(202, "upper", result->upper);
	deserializer.ReadProperty(203, "lower_inclusive", result->lower_inclusive);
	deserializer.ReadProperty(204, "upper_inclusive", result->upper_inclusive);
	return std::move(result);
}

void BoundCaseExpression::Serialize(Serializer &serializer) const {
	Expression::Serialize(serializer);
	serializer.WriteProperty(200, "return_type", return_type);
	serializer.WriteProperty(201, "case_checks", case_checks);
	serializer.WriteProperty(202, "else_expr", else_expr);
}

unique_ptr<Expression> BoundCaseExpression::Deserialize(Deserializer &deserializer) {
	auto return_type = deserializer.ReadProperty<LogicalType>(200, "return_type");
	auto result = duckdb::unique_ptr<BoundCaseExpression>(new BoundCaseExpression(std::move(return_type)));
	deserializer.ReadProperty(201, "case_checks", result->case_checks);
	deserializer.ReadProperty(202, "else_expr", result->else_expr);
	return std::move(result);
}

void BoundCastExpression::Serialize(Serializer &serializer) const {
	Expression::Serialize(serializer);
	serializer.WriteProperty(200, "child", child);
	serializer.WriteProperty(201, "return_type", return_type);
	serializer.WriteProperty(202, "try_cast", try_cast);
}

unique_ptr<Expression> BoundCastExpression::Deserialize(Deserializer &deserializer) {
	auto child = deserializer.ReadProperty<unique_ptr<Expression>>(200, "child");
	auto return_type = deserializer.ReadProperty<LogicalType>(201, "return_type");
	auto result = duckdb::unique_ptr<BoundCastExpression>(new BoundCastExpression(deserializer.Get<ClientContext &>(), std::move(child), std::move(return_type)));
	deserializer.ReadProperty(202, "try_cast", result->try_cast);
	return std::move(result);
}

void BoundColumnRefExpression::Serialize(Serializer &serializer) const {
	Expression::Serialize(serializer);
	serializer.WriteProperty(200, "return_type", return_type);
	serializer.WriteProperty(201, "binding", binding);
	serializer.WriteProperty(202, "depth", depth);
}

unique_ptr<Expression> BoundColumnRefExpression::Deserialize(Deserializer &deserializer) {
	auto return_type = deserializer.ReadProperty<LogicalType>(200, "return_type");
	auto binding = deserializer.ReadProperty<ColumnBinding>(201, "binding");
	auto depth = deserializer.ReadProperty<idx_t>(202, "depth");
	auto result = duckdb::unique_ptr<BoundColumnRefExpression>(new BoundColumnRefExpression(std::move(return_type), binding, depth));
	return std::move(result);
}

void BoundComparisonExpression::Serialize(Serializer &serializer) const {
	Expression::Serialize(serializer);
	serializer.WriteProperty(200, "left", left);
	serializer.WriteProperty(201, "right", right);
}

unique_ptr<Expression> BoundComparisonExpression::Deserialize(Deserializer &deserializer) {
	auto left = deserializer.ReadProperty<unique_ptr<Expression>>(200, "left");
	auto right = deserializer.ReadProperty<unique_ptr<Expression>>(201, "right");
	auto result = duckdb::unique_ptr<BoundComparisonExpression>(new BoundComparisonExpression(deserializer.Get<ExpressionType>(), std::move(left), std::move(right)));
	return std::move(result);
}

void BoundConjunctionExpression::Serialize(Serializer &serializer) const {
	Expression::Serialize(serializer);
	serializer.WriteProperty(200, "children", children);
}

unique_ptr<Expression> BoundConjunctionExpression::Deserialize(Deserializer &deserializer) {
	auto result = duckdb::unique_ptr<BoundConjunctionExpression>(new BoundConjunctionExpression(deserializer.Get<ExpressionType>()));
	deserializer.ReadProperty(200, "children", result->children);
	return std::move(result);
}

void BoundConstantExpression::Serialize(Serializer &serializer) const {
	Expression::Serialize(serializer);
	serializer.WriteProperty(200, "value", value);
}

unique_ptr<Expression> BoundConstantExpression::Deserialize(Deserializer &deserializer) {
	auto value = deserializer.ReadProperty<Value>(200, "value");
	auto result = duckdb::unique_ptr<BoundConstantExpression>(new BoundConstantExpression(value));
	return std::move(result);
}

void BoundDefaultExpression::Serialize(Serializer &serializer) const {
	Expression::Serialize(serializer);
	serializer.WriteProperty(200, "return_type", return_type);
}

unique_ptr<Expression> BoundDefaultExpression::Deserialize(Deserializer &deserializer) {
	auto return_type = deserializer.ReadProperty<LogicalType>(200, "return_type");
	auto result = duckdb::unique_ptr<BoundDefaultExpression>(new BoundDefaultExpression(std::move(return_type)));
	return std::move(result);
}

void BoundLambdaExpression::Serialize(Serializer &serializer) const {
	Expression::Serialize(serializer);
	serializer.WriteProperty(200, "return_type", return_type);
	serializer.WriteProperty(201, "lambda_expr", lambda_expr);
	serializer.WriteProperty(202, "captures", captures);
	serializer.WriteProperty(203, "parameter_count", parameter_count);
}

unique_ptr<Expression> BoundLambdaExpression::Deserialize(Deserializer &deserializer) {
	auto return_type = deserializer.ReadProperty<LogicalType>(200, "return_type");
	auto lambda_expr = deserializer.ReadProperty<unique_ptr<Expression>>(201, "lambda_expr");
	auto captures = deserializer.ReadProperty<vector<unique_ptr<Expression>>>(202, "captures");
	auto parameter_count = deserializer.ReadProperty<idx_t>(203, "parameter_count");
	auto result = duckdb::unique_ptr<BoundLambdaExpression>(new BoundLambdaExpression(deserializer.Get<ExpressionType>(), std::move(return_type), std::move(lambda_expr), parameter_count));
	result->captures = std::move(captures);
	return std::move(result);
}

void BoundLambdaRefExpression::Serialize(Serializer &serializer) const {
	Expression::Serialize(serializer);
	serializer.WriteProperty(200, "return_type", return_type);
	serializer.WriteProperty(201, "binding", binding);
	serializer.WriteProperty(202, "lambda_idx", lambda_idx);
	serializer.WriteProperty(203, "depth", depth);
}

unique_ptr<Expression> BoundLambdaRefExpression::Deserialize(Deserializer &deserializer) {
	auto return_type = deserializer.ReadProperty<LogicalType>(200, "return_type");
	auto binding = deserializer.ReadProperty<ColumnBinding>(201, "binding");
	auto lambda_idx = deserializer.ReadProperty<idx_t>(202, "lambda_idx");
	auto depth = deserializer.ReadProperty<idx_t>(203, "depth");
	auto result = duckdb::unique_ptr<BoundLambdaRefExpression>(new BoundLambdaRefExpression(std::move(return_type), binding, lambda_idx, depth));
	return std::move(result);
}

void BoundOperatorExpression::Serialize(Serializer &serializer) const {
	Expression::Serialize(serializer);
	serializer.WriteProperty(200, "return_type", return_type);
	serializer.WriteProperty(201, "children", children);
}

unique_ptr<Expression> BoundOperatorExpression::Deserialize(Deserializer &deserializer) {
	auto return_type = deserializer.ReadProperty<LogicalType>(200, "return_type");
	auto result = duckdb::unique_ptr<BoundOperatorExpression>(new BoundOperatorExpression(deserializer.Get<ExpressionType>(), std::move(return_type)));
	deserializer.ReadProperty(201, "children", result->children);
	return std::move(result);
}

void BoundParameterExpression::Serialize(Serializer &serializer) const {
	Expression::Serialize(serializer);
	serializer.WriteProperty(200, "identifier", identifier);
	serializer.WriteProperty(201, "return_type", return_type);
	serializer.WriteProperty(202, "parameter_data", parameter_data);
}

unique_ptr<Expression> BoundParameterExpression::Deserialize(Deserializer &deserializer) {
	auto identifier = deserializer.ReadProperty<string>(200, "identifier");
	auto return_type = deserializer.ReadProperty<LogicalType>(201, "return_type");
	auto parameter_data = deserializer.ReadProperty<shared_ptr<BoundParameterData>>(202, "parameter_data");
	auto result = duckdb::unique_ptr<BoundParameterExpression>(new BoundParameterExpression(deserializer.Get<bound_parameter_map_t &>(), std::move(identifier), std::move(return_type), std::move(parameter_data)));
	return std::move(result);
}

void BoundReferenceExpression::Serialize(Serializer &serializer) const {
	Expression::Serialize(serializer);
	serializer.WriteProperty(200, "return_type", return_type);
	serializer.WriteProperty(201, "index", index);
}

unique_ptr<Expression> BoundReferenceExpression::Deserialize(Deserializer &deserializer) {
	auto return_type = deserializer.ReadProperty<LogicalType>(200, "return_type");
	auto index = deserializer.ReadProperty<idx_t>(201, "index");
	auto result = duckdb::unique_ptr<BoundReferenceExpression>(new BoundReferenceExpression(std::move(return_type), index));
	return std::move(result);
}

void BoundUnnestExpression::Serialize(Serializer &serializer) const {
	Expression::Serialize(serializer);
	serializer.WriteProperty(200, "return_type", return_type);
	serializer.WriteProperty(201, "child", child);
}

unique_ptr<Expression> BoundUnnestExpression::Deserialize(Deserializer &deserializer) {
	auto return_type = deserializer.ReadProperty<LogicalType>(200, "return_type");
	auto result = duckdb::unique_ptr<BoundUnnestExpression>(new BoundUnnestExpression(std::move(return_type)));
	deserializer.ReadProperty(201, "child", result->child);
	return std::move(result);
}

} // namespace duckdb
