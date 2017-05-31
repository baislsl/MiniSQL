create table student (
		sno char(8),
		sname char(16) unique,
		sage int unique,
		sgender char (1),
		primary key( sage,sno)
);
select * from student;
select sno, sage from student;
insert into student values ( "gfdg", "sname_v", 54, "g");

0xaa55aa55 : 2857740885