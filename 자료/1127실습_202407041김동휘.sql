DESC skz2_emp;

SELECT DBMS_METADATA.GET_DDL('TABLE', 'SKZ2_EMP') FROM DUAL;

COMMENT ON TABLE skz2_dept IS '부서 테이블';
COMMENT ON COLUMN skz2_dept.deptno IS '부서번호';
COMMENT ON COLUMN skz2_dept.dname IS '부서명';
COMMENT ON COLUMN skz2_dept.loc IS '부서위치';

CREATE TABLE skz2_emp_sal_high
AS
SELECT * FROM skz2_emp WHERE sal >= 2000;

ALTER TABLE skz2_dept
ADD budget number(10,2) default(0);

ALTER TABLE skz2_dept
RENAME COLUMN budget TO dept_budget;

ALTER TABLE skz2_dept
MODIFY dept_budget number(12,2);

ALTER TABLE skz2_dept
DROP COLUMN dept_budget;

ALTER TABLE skz2_emp_sal_high
RENAME TO skz2_sal_high;

CREATE TABLE tmp (
    tmpid number(2)) ;
DROP TABLE tmp;

INSERT INTO skz2_dept (deptno, dname, loc)
VALUES (10, 'accounting', 'New York');

COMMIT;

INSERT INTO skz2_dept(deptno,loc,dname)
VALUES (20, 'Dellas', 'Research');

COMMIT;

INSERT INTO skz_dept (deptno, dname, loc)
VALUES (30, 'Sales', NULL);
COMMIT;

INSERT INTO skz2_sal_high
SELECT * FROM skz2_emp WHERE sal >= 1500 AND sal < 2000;

UPDATE skz2_dept
SET loc = 'JEJU'
WHERE deptno = 10;

COMMIT;

UPDATE skz2_dept
SET loc = 'SEOUL';

COMMIT;

UPDATE skz2_emp
SET sal = sal * 1.1
WHERE job = 'SALESMAN';

commit;

UPDATE skz2_emp
SET job = 'RESEARCH'
WHERE job = 'ANALYST';

COMMIT;

UPDATE skz2_emp
SET sal = sal * 1.1
WHERE sal = ( SELECT MIN(sal)
                FROM skz2_emp );
COMMIT;

UPDATE skz2_emp
SET sal = (SELECT sal FROM skz2_emp WHERE ename = 'BLAKE')
WHERE sal = ( SELECT MIN(sal)
                FROM skz2_emp );
                
COMMIT;

INSERT INTO skz2_dept
VALUES (50, 'E_COMMERCE', 'LONDON');

COMMIT;

SELECT * FROM skz2_dept;

DELETE FROM skz2_dept
WHERE deptno = 50;

COMMIT;
