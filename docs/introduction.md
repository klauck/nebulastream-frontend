# Introduction
This parser library converts SQL strings (e.g., select name from students where id=42) to SQL statement objects, for which members (e.g., tables in the from clause) can be directly accessed.
The parser code is based on (extracted from) DuckDB (https://github.com/duckdb/duckdb/tree/main/third_party/libpg_query), which in turn is based on the PostgreSQL parser (https://github.com/pganalyze/libpg_query).

## Purpose
Nebula Parser is created to enhance the functionality of the Nebula Stream project. Currently, Nebula Stream only supports stream queries, which come with many limitations. To address these limitations, the Nebula SQL Parser is developed.

## Features
Nebula Parser is built on top of the `Postgres Parser`, which offers a rich set of query parsing capabilities. All queries are first parsed by the Postgres Parser and then transformed by the Nebula Parser, ensuring optimal performance.

## Supported Queries
**Currently, the Nebula SQL Parser is able to parse the following type of queries:**

```sql
select * from users --select query with all columns selections
select id from users --simple query with single select
select id, first_name from users --query with multiple select columns
select u.id, u.name from users --query with column specific table selection
select first_name as name from users --select query with column alias
select first_name as name from users where id = 1 --select query with where clause integer comparison
select first_name as name from users where first_name = 'stefan' --select query with where clause string comparison
select name from users where name = 'stefan' limit 10 offset 10 -- select query with limit
SELECT * FROM t1 UNION SELECT * FROM t2 col1; -- union query
SELECT MAX(a) from t1; -- aggregate query max
SELECT AVG(a) from t1; -- aggregate query avg
SELECT COUNT(a) from t1; -- aggregate query count
SELECT SUM(a) from t1; -- aggregate query sum
```

While many other query may parse successfully, it’s because our parser is based on PostgreSQL's parser. Any unsupported queries will be parsed by the PostgreSQL parser. However, these queries may not be transformed by the Nebula Parser, which could lead to the following error:

```text
nebula::NotImplementedException: Transform expression is not implemented
```

Currently, these types of queries are unsupported, and parsing them may raise exceptions.

```sql
SELECT * from "table" JOIN table2 ON a = b WHERE (b OR NOT a) AND a = 12.5
(SELECT a FROM foo WHERE a > 12 OR b > 3 AND c NOT LIKE 's%' LIMIT 10);
SELECT * FROM t WHERE a BETWEEN 1 and c;
SELECT * FROM t WHERE a = ? AND b = ?;
SELECT City.name, Product.category, SUM(price) FROM fact INNER JOIN City ON fact.city_id = City.id INNER JOIN Product ON fact.product_id = Product.id GROUP BY City.name, Product.category;
SELECT SUBSTR(a, 3, 5) FROM t;
SELECT * FROM t WHERE a = DATE '1996-12-31';
SELECT t1.a, t1.b, t2.c FROM "table" AS t1 JOIN (SELECT * FROM foo JOIN bar ON foo.id = bar.id) t2 ON t1.a = t2.b WHERE (t1.b OR NOT t1.a) AND t2.c = 12.5
SELECT * FROM t1 JOIN t2 ON c1 = c2;
SELECT a, SUM(b) FROM t2 GROUP BY a HAVING SUM(b) > 100;
CREATE TABLE students (name TEXT, student_number INTEGER, city TEXT, grade DOUBLE, credits BIGINT)
CREATE TABLE students (name TEXT, student_number INTEGER NOT NULL, city TEXT, grade DOUBLE PRIMARY KEY UNIQUE)
CREATE TABLE teachers (name VARCHAR(30), student_number LONG, city CHAR(10), grade FLOAT)
CREATE TABLE teachers (name VARCHAR(30), student_number LONG, PRIMARY KEY (name, student_number), city CHAR(10), grade FLOAT)
CREATE TABLE teachers (name CHARACTER VARYING(30));
CREATE TABLE students_2 AS SELECT * FROM students
CREATE TABLE students_3 AS SELECT city, grade FROM students WHERE grade > 3.0
CREATE TABLE students (date_of_birth DATE, matriculation_date DATETIME, graduation_date TIMESTAMP, graduated BOOLEAN);
SELECT * FROM t WHERE a = DATE '2000-01-01' + INTERVAL '30 DAYS';
SELECT * FROM t WHERE a = DATE '2000-01-01' + INTERVAL '10' DAY;
--SELECT * FROM t WHERE a BETWEEN '2000-01-01' AND DATE '2000-01-01' - 1 MONTH;
SELECT (CAST('2002-5-01' as DATE) + INTERVAL '60 days');
SELECT CAST(student.student_number as BIGINT) FROM student;
SELECT student.name AS character FROM student; 
SELECT * FROM test WHERE id = 1 FOR UPDATE;
SELECT * FROM test WHERE id = 1 FOR SHARE;
SELECT * FROM test WHERE id = 1 FOR NO KEY UPDATE;
SELECT * FROM test WHERE id = 1 FOR KEY SHARE;
SELECT * FROM test WHERE id = 1 FOR UPDATE SKIP LOCKED;
SELECT * FROM test WHERE id = 1 FOR UPDATE NOWAIT;
SELECT * FROM test1, test2 WHERE test1.id = 10 FOR UPDATE OF test1;
SELECT * FROM test1, test2 WHERE test2.val = 2 FOR SHARE OF test1, test2;
SELECT * FROM test1, test2 WHERE test2.val = 2 FOR UPDATE OF test1 FOR SHARE OF test2; 
SELECT test1, sum(sum(test2)) OVER (PARTITION BY test3 ORDER BY test4 ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW) an_alias FROM test;
SELECT sum(test2)/sum(sum(test2)) OVER (PARTITION BY test1) FROM test GROUP BY test3;
SELECT test1, sum(sum(test2)) OVER (PARTITION BY test3, test4 ORDER BY test5, test6 ROWS BETWEEN 1 PRECEDING AND 2 FOLLOWING) FROM test;
SELECT test1, rank() OVER (ORDER BY test2 DESC, test3 ASC) rnk FROM test;
SELECT rank() OVER () FROM test;
SELECT rank() OVER (PARTITION BY test1) FROM test;
SELECT rank() OVER (PARTITION BY test1 ORDER BY test2) FROM test;
```