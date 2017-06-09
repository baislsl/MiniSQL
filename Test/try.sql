create table student (
		sno char(8),
		sname char(16) unique,
		sage int unique,
		sgender char (1),
		primary key( sage,sno)
);
insert into student values ( "gfdg", "sname_v", 54, "g");

insert into student values ( "gfdg", "sname_v", 54, "g");
create index  student_index on student(sname );
drop index student_index;
create table primary_test (
    wonder char(100),
    id      int,
    primary key(wonder)
);
drop table      primary_test ;
execfile "/home/baislsl/c_c++/MiniSQL/Test/test.sql";
drop index student_index;
create index  student_index on student(sname );


delete from student;
insert into student values ( "gfdg", "sname_v", 54, "g");
insert into student values ( "gfdg", "sname_v", 55, "g");
delete from student where sage = 54;
select sno, sage from student where sage > 53;
select * from student;