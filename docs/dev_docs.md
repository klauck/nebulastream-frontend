# Nebula Parser Documentation

## Overview
The Nebula parser is designed to handle SQL queries, transform them into nodes. The parser includes various components such as SQL statements, query nodes, expressions, and result modifiers. This documentation provides an overview of the classes, enums, and methods used within the Nebula parser.

## Components

### 1. `Parser`
The `Parser` class is responsible for parsing input SQL queries into a vector of SQL statements.

```cpp
class Parser
{
public:
    Parser();
    virtual ~Parser() = default;
    virtual bool parse(const std::string& input);
    virtual void PrintStatements();
    std::vector<std::unique_ptr<SQLStatement>> statements;
};
```
- **Methods**:
    - `bool parse(const std::string& input)`: Parses the input SQL string and generates a vector of SQL statements.
    - `void PrintStatements()`: Prints parsed SQL statements into the console.

### 2. `ResultModifier`
The `ResultModifier` class represents modifiers like `LIMIT`, `ORDER BY`, etc., which modify the output of a query.

```cpp
class ResultModifier
{
public:
    explicit ResultModifier(ResultModifierType type);
    virtual std::string ToString() = 0;
    ResultModifierType type;
    template <class TARGET>
    TARGET& Cast();
};
```
- **Methods**:
    - `std::string ToString()`: Converts the modifier into a string representation.

### 3. `LimitModifier`
Represents a limit modifier in a query, i.e., `LIMIT` and `OFFSET` clauses.

```cpp
class LimitModifier : public ResultModifier
{
public:
    explicit LimitModifier();
    explicit LimitModifier(int offset, int limit);
    int offset = 0;
    int limit = 0;
    std::string ToString() override;
};
```
- **Methods**:
    - `LimitModifier(int offset, int limit)`: Constructor for creating a `LIMIT` and `OFFSET` modifier.
    - `std::string ToString()`: Converts the modifier into a string representation.

### 4. `SetOperationNode`
Represents set operations, e.g., `UNION`, `EXCEPT`, etc.

```cpp
class SetOperationNode : public QueryNode
{
public:
    static constexpr const QueryNodeType TYPE = QueryNodeType::SET_OPERATION_NODE;
    SetOperationNode();
    SetOperationType setop_type = SetOperationType::NONE;
    bool setop_all = false;
    std::unique_ptr<QueryNode> left;
    std::unique_ptr<QueryNode> right;
    std::string ToString() override;
};
```
- **Methods**:
    - `std::string ToString()`: Converts the set operation node into a string representation.

### 5. `SelectNode`
Represents a `SELECT` query node with its various components like the `FROM` clause, `WHERE` clause, etc.

```cpp
class SelectNode : public QueryNode
{
public:
    SelectNode();
    static constexpr const QueryNodeType TYPE = QueryNodeType::SELECT_NODE;
    std::vector<std::unique_ptr<ParsedExpression>> select_list;
    std::vector<std::unique_ptr<TableRef>> from_tables;
    std::unique_ptr<ParsedExpression> where_clause;
    std::string ToString() override;
};
```
- **Methods**:
    - `std::string ToString()`: Converts the `SELECT` node into a string representation.

### 6. `Transformer`
The `Transformer` class is responsible for transforming parsed PostgreSQL parse tree elements into Nebula's internal query node representations.

```cpp
class Transformer
{
public:
    Transformer() = default;
    virtual ~Transformer() = default;

    virtual bool TransformParseTree(pgquery::PGList* tree, std::vector<std::unique_ptr<SQLStatement>>& statements);
    virtual Transformer clone();

    template <class T>
    static T& PGCast(pgquery::PGNode& node);

    template <class T>
    static std::unique_ptr<T> PGPointerCast(void* ptr);

private:
    virtual std::unique_ptr<SQLStatement> TransformStatement(pgquery::PGNode* node);
    virtual std::unique_ptr<SQLStatement> TransformStatementInternal(pgquery::PGNode* node);
    virtual std::unique_ptr<SelectStatement> TransformSelectStatement(pgquery::PGSelectStmt& statement);
    virtual std::unique_ptr<ParsedExpression> TransformExpression(pgquery::PGNode* node);
    virtual std::unique_ptr<ParsedExpression> TransformAExpression(pgquery::PGAExpr& root);
    virtual std::unique_ptr<ParsedExpression> TransformColumnRef(pgquery::PGColumnRef& root);
    virtual std::unique_ptr<ParsedExpression> TransformResTarget(pgquery::PGResTarget& target);
    virtual std::vector<std::unique_ptr<ParsedExpression>> TransformExpressionList(pgquery::PGList* list);
    virtual std::unique_ptr<ParsedExpression> TransformConstant(pgquery::PGAConst& con);
    virtual std::unique_ptr<ParsedExpression> TransformBool(pgquery::PGBoolExpr& expr);
    virtual std::unique_ptr<ParsedExpression> TransformBinaryOperator(std::string& op, std::unique_ptr<ParsedExpression> left, std::unique_ptr<ParsedExpression> right);
    virtual void TransformResultModifiers(pgquery::PGSelectStmt& root, QueryNode& node);
    virtual std::unique_ptr<ParsedExpression> TransformFunction(pgquery::PGFuncCall& func);
    virtual std::unique_ptr<TableRef> TransformSubSelect(pgquery::PGRangeSubselect& root);
    virtual std::unique_ptr<TableRef> TransformRangeVar(pgquery::PGRangeVar* range);
    virtual std::unique_ptr<TableRef> TransformTableRefNode(pgquery::PGNode* node);
    virtual std::vector<std::unique_ptr<TableRef>> TransformFromClause(pgquery::PGList* from);
    std::unique_ptr<QueryNode> TransformSelectNode(pgquery::PGSelectStmt& statement);
};
```
- **Methods**:
  - `bool TransformParseTree(pgquery::PGList* tree, std::vector<std::unique_ptr<SQLStatement>>& statements)`: Converts a PostgreSQL parse tree into Nebula's internal representation.
  - `Transformer clone()`: Clones the current transformer instance.
  - `template <class T> static T& PGCast(pgquery::PGNode& node)`: Casts a `PGNode` to the specified type `T`.
  - `template <class T> static std::unique_ptr<T> PGPointerCast(void* ptr)`: Casts a pointer to a unique pointer of type `T`.
  - `std::unique_ptr<SQLStatement> TransformStatement(pgquery::PGNode* node)`: Transforms a generic PostgreSQL statement node into an SQL statement.
  - `std::unique_ptr<SQLStatement> TransformStatementInternal(pgquery::PGNode* node)`: Handles the internal transformation logic for a statement node.
  - `std::unique_ptr<SelectStatement> TransformSelectStatement(pgquery::PGSelectStmt& statement)`: Transforms a `SELECT` statement node.
  - `std::unique_ptr<ParsedExpression> TransformExpression(pgquery::PGNode* node)`: Transforms a generic PostgreSQL expression node into a parsed expression.
  - `std::unique_ptr<ParsedExpression> TransformAExpression(pgquery::PGAExpr& root)`: Transforms a Star (*) expression node.
  - `std::unique_ptr<ParsedExpression> TransformColumnRef(pgquery::PGColumnRef& root)`: Transforms a column reference node.
  - `std::unique_ptr<ParsedExpression> TransformResTarget(pgquery::PGResTarget& target)`: Transforms a result target node.
  - `std::vector<std::unique_ptr<ParsedExpression>> TransformExpressionList(pgquery::PGList* list)`: Transforms a list of expressions.
  - `std::unique_ptr<ParsedExpression> TransformConstant(pgquery::PGAConst& con)`: Transforms a constant value node.
  - `std::unique_ptr<ParsedExpression> TransformBool(pgquery::PGBoolExpr& expr)`: Transforms a boolean expression node.
  - `std::unique_ptr<ParsedExpression> TransformBinaryOperator(std::string& op, std::unique_ptr<ParsedExpression> left, std::unique_ptr<ParsedExpression> right)`: Transforms a binary operator expression.
  - `void TransformResultModifiers(pgquery::PGSelectStmt& root, QueryNode& node)`: Transforms result modifiers for a `SELECT` statement.
  - `std::unique_ptr<ParsedExpression> TransformFunction(pgquery::PGFuncCall& func)`: Transforms a function call expression, i.e, `Count(*)`.
  - `std::unique_ptr<TableRef> TransformSubSelect(pgquery::PGRangeSubselect& root)`: Transforms a sub-select node into a table reference.
  - `std::unique_ptr<TableRef> TransformRangeVar(pgquery::PGRangeVar* range)`: Transforms a range variable node into a table reference.
  - `std::unique_ptr<TableRef> TransformTableRefNode(pgquery::PGNode* node)`: Transforms a generic table reference node.
  - `std::vector<std::unique_ptr<TableRef>> TransformFromClause(pgquery::PGList* from)`: Transforms the `FROM` clause of a `SELECT` statement.
  - `std::unique_ptr<QueryNode> TransformSelectNode(pgquery::PGSelectStmt& statement)`: Transforms a `SELECT` statement into a query node.
### 7. `SQLStatement`
The base class for all SQL statements.

```cpp
class SQLStatement
{
public:
    explicit SQLStatement(StatementType type);
    virtual ~SQLStatement() = default;
    virtual std::string ToString() = 0;
    virtual void Print() = 0;
};
```
- **Methods**:
    - `std::string ToString()`: Converts the SQL statement to a string.
    - `void Print()`: Prints the SQL statement.

### 8. `SelectStatement`
Represents a `SELECT` SQL statement.

```cpp
class SelectStatement : public SQLStatement
{
public:
    SelectStatement();
    std::unique_ptr<QueryNode> node;
    void Print() override;
    std::string ToString() override;
};
```
- **Methods**:
    - `std::string ToString()`: Converts the `SELECT` statement into a string representation.

### 9. Expressions
Different classes represent different types of parsed expressions:

- **ParsedExpression**: Base class for all parsed expressions.
- **ConstantExpression**: Represents a constant value.
- **ColumnExpression**: Represents a column reference.
- **FunctionExpression**: Represents a function call.
- **StarExpression**: Represents a `*` in the query.
- **ComparisonExpression**: Represents comparison expressions like `=`, `<`, `>`, etc.
- **ConjunctionExpression**: Represents conjunctions like `AND`, `OR`.

### 10. Exception Handling
The Nebula parser defines custom exception classes for handling errors.

```cpp
class NotImplementedException : public std::logic_error
{
public:
    NotImplementedException(const std::string msg);
};

class InvalidOperationException : public std::logic_error
{
public:
    InvalidOperationException(const std::string msg);
};
```

## Enums

### 1. `ResultModifierType`
Enum representing different types of result modifiers.

- `LIMIT_MODIFIER`
- `ORDER_MODIFIER`
- `DISTINCT_MODIFIER`

### 2. `SetOperationType`
Enum representing different types of set operations.

- `UNION`
- `EXCEPT`
- `INTERSECT`

### 3. `QueryNodeType`
Enum representing different types of query nodes.

- `SELECT_NODE`
- `SET_OPERATION_NODE`

### 4. `ExpressionType` and `ExpressionClass`
Enums representing different types of expressions and expression classes.

- `COMPARE_EQUAL`, `COMPARE_LESSTHAN`, `CONJUNCTION_AND`, etc.
- `FUNCTION`, `CONSTANT`, `COLUMN_REF`, etc.
