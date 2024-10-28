#include "tpch_queries.hpp"

namespace nebula_benchmark
{

const std::string tpch_query_1 =
R"(SELECT l_returnflag, l_linestatus, SUM(l_quantity) as sum_qty, SUM(l_extendedprice) as sum_base_price,
      SUM(l_extendedprice*(1-l_discount)) as sum_disc_price,
      SUM(l_extendedprice*(1-l_discount)*(1+l_tax)) as sum_charge, AVG(l_quantity) as avg_qty,
      AVG(l_extendedprice) as avg_price, AVG(l_discount) as avg_disc, COUNT(*) as count_order
      FROM lineitem
      WHERE l_shipdate <= ?
      GROUP BY l_returnflag, l_linestatus
      ORDER BY l_returnflag, l_linestatus;)";


const char* const tpch_query_2 =
    R"(SELECT s_acctbal, s_name, n_name, p_partkey, p_mfgr, s_address, s_phone, s_comment
       FROM part, supplier, partsupp, nation, region
       WHERE p_partkey = ps_partkey AND s_suppkey = ps_suppkey AND p_size = ? AND p_type like ? AND
       s_nationkey = n_nationkey AND n_regionkey = r_regionkey AND r_name = ? AND
       ps_supplycost = (SELECT min(ps_supplycost) FROM partsupp, supplier, nation, region
       WHERE p_partkey = ps_partkey AND s_suppkey = ps_suppkey AND s_nationkey = n_nationkey
       AND n_regionkey = r_regionkey AND r_name = ?) ORDER BY s_acctbal DESC, n_name, s_name, p_partkey LIMIT 100;)";

const char* const tpch_query_3 =
    R"(SELECT l_orderkey, SUM(l_extendedprice*(1-l_discount)) as revenue, o_orderdate, o_shippriority
      FROM customer, orders, lineitem
      WHERE c_mktsegment = ? AND c_custkey = o_custkey AND l_orderkey = o_orderkey
      AND o_orderdate < ? AND l_shipdate > ?
      GROUP BY l_orderkey, o_orderdate, o_shippriority
      ORDER BY revenue DESC, o_orderdate
      LIMIT 10;)";

const char* const tpch_query_4 =
    R"(SELECT o_orderpriority, count(*) as order_count FROM orders WHERE o_orderdate >= ? AND
    o_orderdate < ? AND exists (
    SELECT * FROM lineitem WHERE l_orderkey = o_orderkey AND l_commitdate < l_receiptdate)
    GROUP BY o_orderpriority ORDER BY o_orderpriority;)";

const char* const tpch_query_5 =
    R"(SELECT n_name, SUM(l_extendedprice * (1 - l_discount)) as revenue
      FROM customer, orders, lineitem, supplier, nation, region
      WHERE c_custkey = o_custkey AND l_orderkey = o_orderkey AND l_suppkey = s_suppkey AND c_nationkey = s_nationkey
      AND s_nationkey = n_nationkey AND n_regionkey = r_regionkey AND r_name = ? AND o_orderdate >= ?
      AND o_orderdate < ?
      GROUP BY n_name
      ORDER BY revenue DESC;)";

const char* const tpch_query_6 =
    R"(SELECT sum(l_extendedprice*l_discount) AS revenue
      FROM lineitem
      WHERE l_shipdate >= ? AND l_shipdate < ?
      AND l_discount BETWEEN ? - 0.01 AND ? + 0.01001 AND l_quantity < ?;)";

const char* const tpch_query_7 =
    R"(SELECT
          supp_nation,
          cust_nation,
          l_year,
          SUM(volume) as revenue
      FROM
          (SELECT
              n1.n_name as supp_nation,
              n2.n_name as cust_nation,
              SUBSTR(l_shipdate, 1, 4) as l_year,
              l_extendedprice * (1 - l_discount) as volume
          FROM
              supplier,
              lineitem,
              orders,
              customer,
              nation n1,
              nation n2
          WHERE
              s_suppkey = l_suppkey AND
              o_orderkey = l_orderkey AND
              c_custkey = o_custkey AND
              s_nationkey = n1.n_nationkey AND
              c_nationkey = n2.n_nationkey AND
              ((n1.n_name = ? AND n2.n_name = ?) OR
               (n1.n_name = ? AND n2.n_name = ?)) AND
              l_shipdate BETWEEN ? AND ?
          ) as shipping
      GROUP BY
          supp_nation, cust_nation, l_year
      ORDER BY
          supp_nation, cust_nation, l_year;)";

const char* const tpch_query_8 =
    R"(SELECT o_year, SUM(case when nation = ? then volume else 0 end) / SUM(volume) as mkt_share
     FROM (SELECT SUBSTR(o_orderdate, 1, 4) as o_year, l_extendedprice * (1-l_discount) as volume,
     n2.n_name as nation FROM part, supplier, lineitem, orders, customer, nation n1, nation n2, region
     WHERE p_partkey = l_partkey AND s_suppkey = l_suppkey AND l_orderkey = o_orderkey AND
     o_custkey = c_custkey AND c_nationkey = n1.n_nationkey AND n1.n_regionkey = r_regionkey AND
     r_name = ? AND s_nationkey = n2.n_nationkey AND o_orderdate between ?
     AND ? AND p_type = ?) as all_nations GROUP BY o_year ORDER BY o_year;)";

const char* const tpch_query_9 =
    R"(SELECT nation, o_year, SUM(amount) as sum_profit FROM (SELECT n_name as nation, SUBSTR(o_orderdate, 1, 4) as o_year,
      l_extendedprice * (1 - l_discount) - ps_supplycost * l_quantity as amount
      FROM part, supplier, lineitem, partsupp, orders, nation WHERE s_suppkey = l_suppkey
      AND ps_suppkey = l_suppkey AND ps_partkey = l_partkey AND p_partkey = l_partkey AND o_orderkey = l_orderkey
      AND s_nationkey = n_nationkey AND p_name like ?) as profit
      GROUP BY nation, o_year ORDER BY nation, o_year DESC;)";

const char* const tpch_query_10 =
    R"(SELECT c_custkey, c_name, SUM(l_extendedprice * (1 - l_discount)) as revenue, c_acctbal, n_name, c_address,
      c_phone, c_comment
      FROM customer, orders, lineitem, nation
      WHERE c_custkey = o_custkey AND l_orderkey = o_orderkey AND o_orderdate >= ?
      AND o_orderdate < ? AND l_returnflag = 'R' AND c_nationkey = n_nationkey
      GROUP BY c_custkey, c_name, c_acctbal, c_phone, n_name, c_address, c_comment
      ORDER BY revenue DESC LIMIT 20;)";

const char* const tpch_query_11 =
    R"(SELECT ps_partkey, SUM(ps_supplycost * ps_availqty) as value FROM partsupp, supplier, nation
      WHERE ps_suppkey = s_suppkey AND s_nationkey = n_nationkey AND n_name = ?
      GROUP BY ps_partkey having SUM(ps_supplycost * ps_availqty) > (
      SELECT SUM(ps_supplycost * ps_availqty) * ? FROM partsupp, supplier, nation
      WHERE ps_suppkey = s_suppkey AND s_nationkey = n_nationkey AND n_name = ?) ORDER BY value DESC;)";

const char* const tpch_query_12 =
    R"(SELECT l_shipmode, SUM(case when o_orderpriority ='1-URGENT' or o_orderpriority ='2-HIGH' then 1 else 0 end)
      as high_line_count, SUM(case when o_orderpriority <> '1-URGENT' AND
      o_orderpriority <> '2-HIGH' then 1 else 0 end) as low_line_count FROM orders, lineitem
      WHERE o_orderkey = l_orderkey AND l_shipmode IN (?, ?) AND l_commitdate < l_receiptdate
      AND l_shipdate < l_commitdate AND l_receiptdate >= ? AND
      l_receiptdate < ? GROUP BY l_shipmode ORDER BY l_shipmode;)";

const char* const tpch_query_13 =
    R"(SELECT c_count, count(*) as custdist FROM (SELECT c_custkey, count(o_orderkey) AS c_count
      FROM customer left outer join orders on c_custkey = o_custkey AND o_comment not like ?
      GROUP BY c_custkey) as c_orders GROUP BY c_count ORDER BY custdist DESC, c_count DESC;)";

const char* const tpch_query_14 =
    R"(SELECT 100.00 * SUM(case when p_type like 'PROMO%' then l_extendedprice*(1-l_discount) else 0 end)
      / SUM(l_extendedprice * (1 - l_discount)) as promo_revenue FROM lineitem, part WHERE l_partkey = p_partkey
      AND l_shipdate >= ? AND l_shipdate < ?;)";

const char* const tpch_query_15 =
    R"(create view revenue_view (supplier_no, total_revenue) as SELECT l_suppkey,
      SUM(l_extendedprice * (1 - l_discount)) FROM lineitem WHERE l_shipdate >= '1996-01-01'
      AND l_shipdate < '1996-04-01' GROUP BY l_suppkey;

      SELECT s_suppkey, s_name, s_address, s_phone, total_revenue FROM supplier, revenue_view
      WHERE s_suppkey = supplier_no AND total_revenue = (SELECT max(total_revenue)
      FROM revenue_view) ORDER BY s_suppkey;

      DROP VIEW revenue_view;)";

const char* const tpch_query_16 =
    R"(SELECT p_brand, p_type, p_size, count(distinct ps_suppkey) as supplier_cnt
      FROM partsupp, part WHERE p_partkey = ps_partkey AND p_brand <> ?
      AND p_type not like ? AND p_size in (?, ?, ?, ?, ?, ?, ?, ?)
      AND ps_suppkey not in (SELECT s_suppkey FROM supplier WHERE s_comment like '%Customer%Complaints%')
      GROUP BY p_brand, p_type, p_size ORDER BY supplier_cnt DESC, p_brand, p_type, p_size;)";

const char* const tpch_query_17 =
    R"(SELECT SUM(l_extendedprice) / 7.0 as avg_yearly FROM lineitem, part WHERE p_partkey = l_partkey
      AND p_brand = ? AND p_container = ? AND l_quantity < (SELECT 0.2 * avg(l_quantity)
      FROM lineitem WHERE l_partkey = p_partkey);)";

const char* const tpch_query_18 =
    R"(SELECT c_name, c_custkey, o_orderkey, o_orderdate, o_totalprice, SUM(l_quantity)
      FROM customer, orders, lineitem WHERE o_orderkey in (SELECT l_orderkey FROM lineitem
      GROUP BY l_orderkey having SUM(l_quantity) > ?) AND c_custkey = o_custkey AND o_orderkey = l_orderkey
      GROUP BY c_name, c_custkey, o_orderkey, o_orderdate, o_totalprice ORDER BY o_totalprice DESC, o_orderdate
      LIMIT 100;)";

const char* const tpch_query_19 =

    R"(SELECT SUM(l_extendedprice * (1 - l_discount) ) as revenue FROM lineitem, part WHERE (( p_partkey = l_partkey AND
      p_brand = ? AND p_container in ( 'SM CASE', 'SM BOX', 'SM PACK', 'SM PKG') AND l_quantity >= ? AND l_quantity
      <= ? + 10 AND p_size between 1 AND 5 AND l_shipmode in ('AIR', 'AIR REG') AND l_shipinstruct = 'DELIVER IN PERSON') or
      (p_partkey = l_partkey AND p_brand = ? AND p_container in ('MED BAG', 'MED BOX', 'MED PKG', 'MED PACK') AND
      l_quantity >= ? AND l_quantity <= ? + 10 AND p_size between 1 AND 10 AND l_shipmode in ('AIR', 'AIR REG') AND
      l_shipinstruct = 'DELIVER IN PERSON') or (p_partkey = l_partkey AND p_brand = ? AND p_container in (
      'LG CASE', 'LG BOX', 'LG PACK', 'LG PKG') AND l_quantity >= ? AND l_quantity <= ? + 10 AND p_size between 1 AND 15
      AND l_shipmode in ('AIR', 'AIR REG') AND l_shipinstruct = 'DELIVER IN PERSON'));)";

const char* const tpch_query_20 =
    R"(SELECT s_name, s_address FROM supplier, nation WHERE s_suppkey in (SELECT ps_suppkey FROM partsupp
      WHERE ps_partkey in (SELECT p_partkey FROM part WHERE p_name like ?) AND ps_availqty >
      (SELECT 0.5 * SUM(l_quantity) FROM lineitem WHERE l_partkey = ps_partkey AND l_suppkey = ps_suppkey AND
      l_shipdate >= ? AND l_shipdate < ?)) AND s_nationkey = n_nationkey
      AND n_name = ? ORDER BY s_name;)";

const char* const tpch_query_21 =
    R"(SELECT s_name, count(*) as numwait FROM supplier, lineitem l1, orders, nation WHERE s_suppkey = l1.l_suppkey
      AND o_orderkey = l1.l_orderkey AND o_orderstatus = 'F' AND l1.l_receiptdate > l1.l_commitdate AND exists
      (SELECT * FROM lineitem l2 WHERE l2.l_orderkey = l1.l_orderkey AND l2.l_suppkey <> l1.l_suppkey) AND not exists
      (SELECT * FROM lineitem l3 WHERE l3.l_orderkey = l1.l_orderkey AND l3.l_suppkey <> l1.l_suppkey AND
      l3.l_receiptdate > l3.l_commitdate ) AND s_nationkey = n_nationkey AND n_name = ? GROUP BY s_name
      ORDER BY numwait DESC, s_name LIMIT 100;)";

const char* const tpch_query_22 =
    R"(SELECT
         cntrycode, COUNT(*) AS numcust, SUM(c_acctbal) AS totacctbal
       FROM
         (SELECT
            SUBSTR(c_phone,1,2) AS cntrycode, c_acctbal
          FROM
            customer
          WHERE
            SUBSTR(c_phone,1,2) IN (?, ?, ?, ?, ?, ?, ?) AND
            c_acctbal > (SELECT
                           AVG(c_acctbal)
                         FROM
                           customer
                         WHERE
                          c_acctbal > 0.00 AND
                          SUBSTR(c_phone,1,2) IN (?, ?, ?, ?, ?, ?, ?)) AND
            NOT EXISTS ( SELECT * FROM orders WHERE o_custkey = c_custkey)
        ) AS custsale
       GROUP BY cntrycode
       ORDER BY cntrycode;)";

std::vector<std::string> TpchQueries::queries = {tpch_query_1, tpch_query_2, tpch_query_3, tpch_query_4, tpch_query_5,
                                                  tpch_query_6, tpch_query_7, tpch_query_8, tpch_query_9, tpch_query_10,
                                                  tpch_query_11, tpch_query_12, tpch_query_13, tpch_query_14, tpch_query_15,
                                                  tpch_query_16, tpch_query_17, tpch_query_18, tpch_query_19, tpch_query_20,
                                                  tpch_query_21, tpch_query_22};
}