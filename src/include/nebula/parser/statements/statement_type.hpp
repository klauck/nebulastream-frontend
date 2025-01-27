// duckdb reference: src/include/duckdb/common/enums/statement_type.hp
#pragma once

#include <string>

namespace nebula
{
enum class StatementType : short
{
    INVALID_STATEMENT, // invalid statement type
    SELECT_STATEMENT, // select statement type
    INSERT_STATEMENT, // insert statement type
    UPDATE_STATEMENT, // update statement type
    CREATE_STATEMENT, // create statement type
    DELETE_STATEMENT, // delete statement type
    PREPARE_STATEMENT, // prepare statement type
    EXECUTE_STATEMENT, // execute statement type
    ALTER_STATEMENT, // alter statement type
    TRANSACTION_STATEMENT, // transaction statement type,
    COPY_STATEMENT, // copy type
    ANALYZE_STATEMENT, // analyze type
    VARIABLE_SET_STATEMENT, // variable set statement type
    CREATE_FUNC_STATEMENT, // create func statement type
    EXPLAIN_STATEMENT, // explain statement type
    DROP_STATEMENT, // DROP statement type
    EXPORT_STATEMENT, // EXPORT statement type
    PRAGMA_STATEMENT, // PRAGMA statement type
    VACUUM_STATEMENT, // VACUUM statement type
    CALL_STATEMENT, // CALL statement type
    SET_STATEMENT, // SET statement type
    LOAD_STATEMENT, // LOAD statement type
    RELATION_STATEMENT,
    EXTENSION_STATEMENT,
    LOGICAL_PLAN_STATEMENT,
    ATTACH_STATEMENT,
    DETACH_STATEMENT,
    MULTI_STATEMENT,
    COPY_DATABASE_STATEMENT,
    UPDATE_EXTENSIONS_STATEMENT,
};

std::string StatementReturnTypeToString(StatementType type);
}
