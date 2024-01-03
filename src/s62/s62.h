//===----------------------------------------------------------------------===//
//
//                         DuckDB
//
// duckdb.h
//
//
//===----------------------------------------------------------------------===//

#pragma once


#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

//===--------------------------------------------------------------------===//
// Type Information
//===--------------------------------------------------------------------===//
typedef uint64_t idx_t;
typedef const char* s62_version;
typedef char* s62_query;
typedef char* s62_plan;
typedef char* s62_label_name;
typedef char* s62_property_name;
typedef char* s62_sql_type;
typedef int s62_property_order;
typedef int s62_precision;
typedef int s62_scale;
typedef size_t s62_num_metadata;
typedef size_t s62_num_properties;
typedef size_t s62_num_rows;
typedef size_t s62_cursor;

typedef enum S62_METADATA_TYPE {
	S62_NODE = 0,
	S62_EDGE = 1,
	S62_OTHER = 2,
} s62_metadata_type;

typedef enum S62_TYPE {
	S62_TYPE_INVALID = 0,
	// bool
	S62_TYPE_BOOLEAN,
	// int8_t
	S62_TYPE_TINYINT,
	// int16_t
	S62_TYPE_SMALLINT,
	// int32_t
	S62_TYPE_INTEGER,
	// int64_t
	S62_TYPE_BIGINT,
	// uint8_t
	S62_TYPE_UTINYINT,
	// uint16_t
	S62_TYPE_USMALLINT,
	// uint32_t
	S62_TYPE_UINTEGER,
	// uint64_t
	S62_TYPE_UBIGINT,
	// float
	S62_TYPE_FLOAT,
	// double
	S62_TYPE_DOUBLE,
	// S62_timestamp, in microseconds
	S62_TYPE_TIMESTAMP,
	// S62_date
	S62_TYPE_DATE,
	// S62_time
	S62_TYPE_TIME,
	// S62_interval
	S62_TYPE_INTERVAL,
	// S62_hugeint
	S62_TYPE_HUGEINT,
	// const char*
	S62_TYPE_VARCHAR,
	// S62_blob
	S62_TYPE_BLOB,
	// decimal
	S62_TYPE_DECIMAL,
	// S62_timestamp, in seconds
	S62_TYPE_TIMESTAMP_S,
	// S62_timestamp, in milliseconds
	S62_TYPE_TIMESTAMP_MS,
	// S62_timestamp, in nanoseconds
	S62_TYPE_TIMESTAMP_NS,
	// enum type, only useful as logical type
	S62_TYPE_ENUM,
	// list type, only useful as logical type
	S62_TYPE_LIST,
	// struct type, only useful as logical type
	S62_TYPE_STRUCT,
	// map type, only useful as logical type
	S62_TYPE_MAP,
	// S62_hugeint
	S62_TYPE_UUID,
	// union type, only useful as logical type
	S62_TYPE_UNION,
	// S62_bit
	S62_TYPE_BIT,
	// S62_ID
	S62_TYPE_ID,
} s62_type;

//! Days are stored as days since 1970-01-01
//! Use the duckdb_from_date/duckdb_to_date function to extract individual information
typedef struct {
	int32_t days;
} s62_date;

typedef struct {
	int32_t year;
	int8_t month;
	int8_t day;
} s62_date_struct;

//! Time is stored as microseconds since 00:00:00
//! Use the duckdb_from_time/duckdb_to_time function to extract individual information
typedef struct {
	int64_t micros;
} s62_time;

typedef struct {
	int8_t hour;
	int8_t min;
	int8_t sec;
	int32_t micros;
} s62_time_struct;

//! Timestamps are stored as microseconds since 1970-01-01
//! Use the duckdb_from_timestamp/duckdb_to_timestamp function to extract individual information
typedef struct {
	int64_t micros;
} s62_timestamp;

typedef struct {
	s62_date_struct date;
	s62_time_struct time;
} s62_timestamp_struct;

typedef struct {
	int32_t months;
	int32_t days;
	int64_t micros;
} s62_interval;

typedef struct {
	uint64_t lower;
	int64_t upper;
} s62_hugeint;

typedef struct {
	uint8_t width;
	uint8_t scale;

	s62_hugeint value;
} s62_decimal;

typedef struct {
	char *data;
	idx_t size;
} s62_string;

/*
    The internal data representation of a VARCHAR/BLOB column
*/
typedef struct {
	union {
		struct {
			uint32_t length;
			char prefix[4];
			char *ptr;
		} pointer;
		struct {
			uint32_t length;
			char inlined[12];
		} inlined;
	} value;
} s62_string_t;

typedef struct {
	uint64_t offset;
	uint64_t length;
} s62_list_entry;

typedef struct _s62_property {
	s62_label_name label_name;
	s62_metadata_type label_type;
	s62_property_order order;
	s62_property_name property_name;
	s62_type property_type;
	s62_sql_type property_sql_type;
	s62_precision precision;
	s62_scale scale;
	struct _s62_property *next;
} s62_property;

typedef struct _s62_prepared_statement {
	s62_query query;
	s62_plan plan;
	s62_num_properties num_properties;
	s62_property *property;
	void *__internal_prepared_statement;
} s62_prepared_statement;

typedef struct _s62_value {
	void *__val;
} * s62_value;

typedef struct _s62_result {
	s62_type data_type;
	s62_sql_type data_sql_type;
	s62_num_rows num_rows;
	void* __internal_data;
	struct _s62_result *next;
} s62_result;

typedef struct _s62_resultset {
	s62_num_properties num_properties;
	s62_result *result;
	struct _s62_resultset *next;
} s62_resultset;

typedef struct _s62_resultset_wrapper {
	s62_num_rows num_total_rows;
	s62_cursor cursor;
	struct _s62_resultset *result_set;
} s62_resultset_wrapper;

typedef struct _s62_metadata {
	s62_label_name label_name;
	s62_metadata_type type;
	struct _s62_metadata *next;
} s62_metadata;

typedef enum {
    S62_SUCCESS = 0,
    S62_ERROR = -1,
} s62_state;

typedef enum {
    S62_CONNECTED = 0,
    S62_NOT_CONNECTED = 1,
} s62_conn_state;

typedef enum {
	S62_MORE_RESULT = 1,
	S62_END_OF_RESULT = 0,
	S62_ERROR_RESULT = -1,
} s62_fetch_state;

typedef enum {
    S62_NO_ERROR = 0,
    S62_ERROR_CONNECTION_FAILED = -1,
	S62_ERROR_INVALID_STATEMENT = -2,
	S62_ERROR_INVALID_PARAMETER_INDEX = -3,
	S62_ERROR_UNSUPPORTED_OPERATION = -4,
	S62_ERROR_INVALID_METADATA = -5,
	S62_ERROR_INVALID_LABEL = -6,
	S62_ERROR_INVALID_PROPERTY = -7,
	S62_ERROR_INVALID_NUMBER_OF_PROPERTIES = -8,
	S62_ERROR_INVALID_PREPARED_STATEMENT = -9,
	S62_ERROR_INVALID_METADATA_TYPE = -10,
	S62_ERROR_INVALID_PLAN = -11,
	S62_ERROR_INVALID_RESULT_SET = -12,
	S62_ERROR_INVALID_COLUMN_INDEX = -13,
	S62_ERROR_INVALID_COLUMN_TYPE = -14,
	S62_ERROR_INVALID_CURSOR = -15,
} s62_error_code;

typedef enum {
	DUCKDB_PENDING_RESULT_READY = 0,
	DUCKDB_PENDING_RESULT_NOT_READY = 1,
	DUCKDB_PENDING_ERROR = 2,
	DUCKDB_PENDING_NO_TASKS_AVAILABLE = 3
} duckdb_pending_state;

//===--------------------------------------------------------------------===//
// Open/Connect
//===--------------------------------------------------------------------===//

s62_state s62_connect(const char *dbname);

void s62_disconnect();

s62_conn_state s62_is_connected();

s62_error_code s62_get_last_error(char **errmsg);

s62_version s62_get_version();

//===--------------------------------------------------------------------===//
// Metadata
//===--------------------------------------------------------------------===//

s62_num_metadata s62_get_metadata_from_catalog(s62_label_name label, bool like_flag, bool filter_flag, s62_metadata **metadata);

s62_state s62_close_metadata(s62_metadata *metadata);

s62_num_properties s62_get_property_from_catalog(s62_label_name label, s62_metadata_type type, s62_property** property);

s62_state s62_close_property(s62_property *property);

//===--------------------------------------------------------------------===//
// s62_query
//===--------------------------------------------------------------------===//

s62_prepared_statement* s62_prepare(s62_query query);

s62_state s62_close_prepared_statement(s62_prepared_statement* prepared_statement);

s62_state s62_bind_value(s62_prepared_statement* prepared_statement, idx_t param_idx, s62_value val);

s62_state s62_bind_boolean(s62_prepared_statement* prepared_statement, idx_t param_idx, bool val);

s62_state s62_bind_int8(s62_prepared_statement* prepared_statement, idx_t param_idx, int8_t val);

s62_state s62_bind_int16(s62_prepared_statement* prepared_statement, idx_t param_idx, int16_t val);

s62_state s62_bind_int32(s62_prepared_statement* prepared_statement, idx_t param_idx, int32_t val);

s62_state s62_bind_int64(s62_prepared_statement* prepared_statement, idx_t param_idx, int64_t val);

s62_state s62_bind_hugeint(s62_prepared_statement* prepared_statement, idx_t param_idx, s62_hugeint val);

s62_state s62_bind_uint8(s62_prepared_statement* prepared_statement, idx_t param_idx, uint8_t val);

s62_state s62_bind_uint16(s62_prepared_statement* prepared_statement, idx_t param_idx, uint16_t val);

s62_state s62_bind_uint32(s62_prepared_statement* prepared_statement, idx_t param_idx, uint32_t val);

s62_state s62_bind_uint64(s62_prepared_statement* prepared_statement, idx_t param_idx, uint64_t val);

s62_state s62_bind_float(s62_prepared_statement* prepared_statement, idx_t param_idx, float val);

s62_state s62_bind_double(s62_prepared_statement* prepared_statement, idx_t param_idx, double val);

s62_state s62_bind_date(s62_prepared_statement* prepared_statement, idx_t param_idx, s62_date val);

s62_state s62_bind_time(s62_prepared_statement* prepared_statement, idx_t param_idx, s62_time val);

s62_state s62_bind_timestamp(s62_prepared_statement* prepared_statement, idx_t param_idx, s62_timestamp val);

s62_state s62_bind_varchar(s62_prepared_statement* prepared_statement, idx_t param_idx, const char *val);

s62_state s62_bind_varchar_length(s62_prepared_statement* prepared_statement, idx_t param_idx, const char *val, idx_t length);

s62_state s62_bind_decimal(s62_prepared_statement* prepared_statement, idx_t param_idx, s62_decimal val);

s62_state s62_bind_null(s62_prepared_statement* prepared_statement, idx_t param_idx);

//===--------------------------------------------------------------------===//
// s62_execute
//===--------------------------------------------------------------------===//

s62_num_rows s62_execute(s62_prepared_statement* prep_query, s62_resultset_wrapper** result_set_wrp);

s62_state s62_close_resultset(s62_resultset_wrapper* result_set_wrp);

s62_fetch_state s62_fetch_next(s62_resultset_wrapper* result_set_wrp);

bool s62_get_bool(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

int8_t s62_get_int8(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

int16_t s62_get_int16(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

int32_t s62_get_int32(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

int64_t s62_get_int64(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

s62_hugeint s62_get_hugeint(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

uint8_t s62_get_uint8(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

uint16_t s62_get_uint16(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

uint32_t s62_get_uint32(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

uint64_t s62_get_uint64(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

float s62_get_float(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

double s62_get_double(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

s62_date s62_get_date(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

s62_time s62_get_time(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

s62_timestamp s62_get_timestamp(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

s62_string s62_get_varchar(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

s62_decimal s62_get_decimal(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

s62_string s62_decimal_to_string (s62_decimal val);

uint64_t s62_get_id(s62_resultset_wrapper* result_set_wrp, idx_t col_idx);

#ifdef __cplusplus
}
#endif
