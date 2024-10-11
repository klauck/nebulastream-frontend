SELECT * FROM users --SELECT query with all columns selections
SELECT id FROM users --simple query with single SELECT
SELECT id, name FROM users --query with multiple SELECT columns
SELECT u.id, u.name FROM users --query with column specific table selection
SELECT first_name as name FROM users --SELECT query with column alias
SELECT first_name as name FROM users WHERE id = 1 --SELECT query with WHERE clause integer comparison
SELECT first_name as name FROM users WHERE first_name = 'stefan' --SELECT query with WHERE clause string comparison
SELECT name FROM users WHERE name = 'stefan' limit 10 offset 10 -- SELECT query with limit
SELECT * FROM t1 UNION SELECT * FROM t2 col1; -- union query