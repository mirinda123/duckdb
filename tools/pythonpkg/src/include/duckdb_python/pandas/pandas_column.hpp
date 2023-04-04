#pragma once

namespace duckdb {

enum class PandasColumnBackend { NUMPY, ARROW };

class PandasColumn {
public:
	PandasColumn(PandasColumnBackend backend) : backend(backend) {
	}
	virtual ~PandasColumn() {
	}

public:
	PandasColumnBackend Backend() const {
		return backend;
	}

protected:
	PandasColumnBackend backend;
};

} // namespace duckdb
