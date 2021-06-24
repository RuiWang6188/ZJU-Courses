<h1 style="text-align:center">Project1 Report</h1>

​																																						3180103650 王睿

### 1. Project Specification

This project focus on storing and processing students score data.

There are 2 input formats: `<student id>, <name>` and `<student id>, <course name>, <marks>`. However, the order as well as the number of those inputs are not known.

Out task is to record all the grades of each student, and compute his/her average score of those courses he/she have taken. In addition, we have to output the average score of each courses as well.

Sample of the input and output format are as follows:

- Sample Input

  ```
  3180111435, Operating System, 34.5
  3180111430, Linear Algebra, 80
  3180111435, Jessie Zhao
  3180111430, Zhiwen Yang
  3180111430, Computer Architecture, 46.5
  3180111434, Linear Algebra, 61.5
  3180111434, Anna Teng
  ```

- Sample output

  ```in
  3180111435, Operating System, 34.5
  3180111430, Linear Algebra, 80
  3180111435, Jessie Zhao
  3180111430, Zhiwen Yang
  3180111430, Computer Architecture, 46.5
  3180111434, Linear Algebra, 61.5
  3180111434, Anna Teng
  ```

For more detailed problem specification, you can access this [link](https://pintia.cn/problem-sets/1371752399094030336/problems/1371752550827171840).

### 2. Class Specification

#### 2.1 Course Class

​	`Course` is a class to store the information of course name, total grade of those students who takes the class get, and the student number. We include the latter two in order to compute the course average score.

The course class is specified as follows:

```C++
class Course {
public:
	string name;
	double totalGrade;
	int studentNum;
	Course(){}
	Course(string courseName) {
		name = courseName;
		totalGrade = 0;
		studentNum = 0;
	}

	bool operator < (const Course& course) const {
		return (name < course.name);
	}
};
```

 Note that the operator redefinition: `bool operator <()` is used to sort in the map structure.

#### 2.2 Student Class

`Student` class is used to store information of each student, including the student ID, student name, the courses he/she have taken and the scores accordingly, also, the average score of that student.

The `Student` class is defined as follows

```C++
class Student {
public:
	string ID;
	string name;
	map<string, double> taken;
	double average;

	Student(){}
	Student(string id, string Name = "") {
		ID = id;
		name = Name;
		average = 0;
		taken.clear();
	}
	
	bool operator < (const Student& stu) const {
		return (ID < stu.ID);
	}
};
```

Note that we used a map STL data structure `taken` to store the information of courses and the score.

And the operator redefinition serve the same functionality as the `Course` class.

Also, I use the default parameter for the Name in the constructor, since it may occur only given the ID.

#### 2.3 Manager Class

Note that whether Course class or Student class, they all just store information, and doesn't process information. We use the Manager class to achieve the goal of modifying and processing information.

The declaration of the Manager Class is as follows:

```C++
Class Manager{
public:
    map<string, Course> all_course;
	map<string, Student> all_student;
    Manager();
    bool isNewStudent(string id);
    bool isNewCourse(string courseName);
    void setStudentName(string id, string name);
    void updateAllStu(string id, Student stu);
    void updateAllCourse(string courseName, Course course);
    void updateStuTaken(string id, string courseName, double grade);
    void updateCourseEnroll(string courseName, double grade);
	bool haveTaken(string ID, string courseName);
}
```

And we'll introduce them one by one.

- two maps: `all_course` and `all_student`

  ​	This is where we store the `Course` object and the `Student` object. 

  - `all_course`: key is the course name, and the value is the Course object accordingly.
  - `all_student`: key is the student ID, and the value is the Student object accordingly.

- Default constructor `Manager()`

  ​	A default constructor, which clear the `all_course` and `all_student`.

- bool function: `isNewStudent()` and `isNewCourse()`

  ​	They are used to judge if a course or a student is first encountered and hasn't been recorded in the map accordingly. We use the method `count()` of map to implement this.

  ```C++
  	bool isNewStudent(string id) {
  		if (all_student.count(id) == 0)
  			return true;
  		else
  			return false;
  	}
  
  	bool isNewCourse(string courseName) {
  		if (all_course.count(courseName) == 0)
  			return true;
  		else
  			return false;
  	}
  ```

- void function: `setStudentName()`

  ​	Since the order of the input is random, so it may occur the situation where the ID is given while the name is unknown. So we need to update the student's name backwards.

  ```c++
  	void setStudentName(string id, string name) {
  		all_student[id].name = name;
  	}
  ```

- void function: `updateAllStu()` and `updateAllCourse()`

  These two member functions are called when we encounter a new student ID or a new course name. It will add the new pair to the `all_student` and `all_course` accordingly.

  ```c++
  	void updateAllStu(string id, Student stu) {
  		if (!isNewStudent(id)) {
  			cout << "ERROR: Already in all_student!" << endl;
  			exit(-1);
  		}
  		else {
  			all_student.insert(make_pair(id, stu));
  		}
  	}
  
  	void updateAllCourse(string courseName, Course course) {
  		if (!isNewCourse(courseName)) {
  			cout << "ERROR: Already in all_course!" << endl;
  			exit(-1);
  		}
  		else {
  			all_course.insert(make_pair(courseName, course));
  		}
  	}
  ```

- void function: `updateStuTaken()`

  ​	This is a function aims to update the `taken` map in the student class.

   	Also we will update the `average` member variable as well.

  ```C++
  	void updateStuTaken(string id, string courseName, double grade) {
  		if (isNewStudent(id)) {
  			cout << "ERROR: New Student!" << endl;
  			exit(-1);
  		}
  		else if (isNewCourse(courseName)) {
  			cout << "ERROR: New Course!" << endl;
  			exit(-1);
  		}
  
  		all_student[id].taken.insert(make_pair(courseName, grade));
  		
  		all_student[id].average = (all_student[id].average * (all_student[id].taken.size() - 1) + grade) / all_student[id].taken.size();
  	}
  ```

- void function: `updateCourseEnroll()`

  This is a function used to update the Course member variables `totalGrade` and `studentNum`

  ```C++
  	void updateCourseEnroll(string courseName, double grade) {
  		if (isNewCourse(courseName)) {
  			cout << "ERROR: New Course!" << endl;
  			exit(-1);
  		}
  		all_course[courseName].studentNum++;
  		all_course[courseName].totalGrade += grade;
  	}
  ```

- bool function: `haveTaken()`

  This is a function used to judge if a student have taken one course.

  ```c++
  	bool haveTaken(string ID, string courseName) {
  		if (all_student[ID].taken.count(courseName) == 0)
  			return false;
  		else
  			return true;
  	}
  ```

  It makes the output of the student grade easier.

### 3. Implementation Details

#### 3.1 Read Input and Process

- I defined an object `manager` which in charge of all the modification and information process.

- I use the method described in XueZaiZheDa to read in the input, and each processed parameter is store in a string vector `a`

  ```c++
  while (getline(cin, line)) {
      if (line.size() < 1)
          continue;
      istringstream f(line);
      vector<string> a;
      a.clear();
      string w;
      while (getline(f, w, ',')) {
          trim(w);
          a.push_back(w);
      }
      ...
  }
  ```

- There are two input formats, two parameters and three parameters

  - 2 parameters

    This part is easy. If the student is already enrolled, then we need to modify its name, otherwise, we define an student object and update the `all_student` map

    ```C++
    		// input: <student id>, <name>
    		if (a.size() == 2) { 
    			string ID = a[0];
    			string stuName = a[1];
    
    			if (manager.isNewStudent(ID)) {
    				Student stu(ID, stuName);
    				manager.updateAllStu(ID, stu);
    			}
    			else {
    				manager.setStudentName(ID, stuName);
    			}
    		}
    ```

  - 3 parameters

    We first examine if the student or course is new, and take the corresponding actions. Then we update the `taken` variable in the student object and the `totalGrade, studentNum` in the course object by calling the member function of ` manager`.  

    ```C++
    		// input: <student id>, <course name>, <marks>
    		else if (a.size() == 3) {
    			string ID = a[0];
    			string courseName = a[1];
    			double grade = string2num<double>(a[2]);
    
    			if (manager.isNewStudent(ID)) {
    				Student stu(ID);
    				manager.updateAllStu(ID, stu);
    			}
    			if (manager.isNewCourse(courseName)) {
    				Course course(courseName);
    				manager.updateAllCourse(courseName, course);
    			}
    
    			manager.updateStuTaken(ID, courseName, grade);
    			manager.updateCourseEnroll(courseName, grade);
    		}
    
    ```

#### 3.2 Output Result

The output session can be divided into 3 parts

- Output the first prompt

  ```c++
  	// print the first prompt line
  	cout << "student id, name";
  	for (map<string, Course>::iterator p = manager.all_course.begin(); p != manager.all_course.end(); p++) {
  		cout << ", " << p->first;
  	}
  	cout << ", average" << endl;
  ```

- Output each student's grades

  For each student, we loop through each course, and using the `haveTaken()` function to judge the output format 

  - taken -> output the grade
  - not taken -> simple output `, `

  ```c++
  	// print the student grades
  	for (map<string, Student>::iterator p = manager.all_student.begin(); p != manager.all_student.end(); p++) {
  		cout << p->first << ", " << p->second.name;
  		string courseName;
  		string ID = p->first;
  
  		for (map<string, Course>::iterator q = manager.all_course.begin(); q != manager.all_course.end(); q++) {
  			courseName = q->first;
  			if (manager.haveTaken(ID, courseName)) {
  				cout << ", " << fixed << setprecision(1) << p->second.taken[courseName];
  			}
  			else {
  				cout << ", ";
  			}
  		}
  
  		cout << ", " << fixed << setprecision(1) << p->second.average << endl;
  	}
  ```

- Output the average grade of each course

  Just loop the `all_course` map and process the grades

  ```c++
  	// print the average score of each course
  	double sum = 0;
  	double val = 0;
  	cout << ", ";
  	for (map<string, Course>::iterator p = manager.all_course.begin(); p != manager.all_course.end(); p++) {
  		val = p->second.totalGrade / p->second.studentNum;
  		sum += val;
  		cout << ", " << fixed << setprecision(1) << val;
  	}
  	cout << ", " << fixed << setprecision(1) << sum / manager.all_course.size() << endl;
  
  ```

### 4. Appendix

source code

```c++
#include<iostream>
#include<sstream>
#include<map>
#include<iomanip>
#include<string>
#include<vector>
#include<algorithm>

using namespace std;

string& trim(string& s);
template <class Type>
Type string2num(const string& str);

class Course {
public:
	string name;
	double totalGrade;
	int studentNum;
	Course(){}
	Course(string courseName) {
		name = courseName;
		totalGrade = 0;
		studentNum = 0;
	}

	bool operator < (const Course& course) const {
		return (name < course.name);
	}
};

class Student {
public:
	string ID;
	string name;
	map<string, double> taken;
	double average;

	Student(){}
	Student(string id, string Name = "") {
		ID = id;
		name = Name;
		average = 0;
		taken.clear();
	}
	
	bool operator < (const Student& stu) const {
		return (ID < stu.ID);
	}
};


class Manager {
public:
	map<string, Course> all_course;
	map<string, Student> all_student;

	Manager(void) {
		all_course.clear();
		all_student.clear();
	}

	bool isNewStudent(string id) {
		if (all_student.count(id) == 0)
			return true;
		else
			return false;
	}

	bool isNewCourse(string courseName) {
		if (all_course.count(courseName) == 0)
			return true;
		else
			return false;
	}

	void setStudentName(string id, string name) {
		all_student[id].name = name;
	}

	void updateAllStu(string id, Student stu) {
		if (!isNewStudent(id)) {
			cout << "ERROR: Already in all_student!" << endl;
			exit(-1);
		}
		else {
			all_student.insert(make_pair(id, stu));
		}
	}

	void updateAllCourse(string courseName, Course course) {
		if (!isNewCourse(courseName)) {
			cout << "ERROR: Already in all_course!" << endl;
			exit(-1);
		}
		else {
			all_course.insert(make_pair(courseName, course));
		}
	}

	void updateCourseEnroll(string courseName, double grade) {
		if (isNewCourse(courseName)) {
			cout << "ERROR: New Course!" << endl;
			exit(-1);
		}
		all_course[courseName].studentNum++;
		all_course[courseName].totalGrade += grade;
	}

	void updateStuTaken(string id, string courseName, double grade) {
		if (isNewStudent(id)) {
			cout << "ERROR: New Student!" << endl;
			exit(-1);
		}
		else if (isNewCourse(courseName)) {
			cout << "ERROR: New Course!" << endl;
			exit(-1);
		}

		all_student[id].taken.insert(make_pair(courseName, grade));
		
		all_student[id].average = (all_student[id].average * (all_student[id].taken.size() - 1) + grade) / all_student[id].taken.size();
	}

	bool haveTaken(string ID, string courseName) {
		if (all_student[ID].taken.count(courseName) == 0)
			return false;
		else
			return true;
	}
};


int main()
{
	string line;
	Manager manager;

	while (getline(cin, line)) {
		if (line.size() < 1)
			continue;
		istringstream f(line);
		vector<string> a;
		a.clear();
		string w;
		while (getline(f, w, ',')) {
			trim(w);
			a.push_back(w);
		}

		// input: <student id>, <name>
		if (a.size() == 2) { 
			string ID = a[0];
			string stuName = a[1];

			if (manager.isNewStudent(ID)) {
				Student stu(ID, stuName);
				manager.updateAllStu(ID, stu);
			}
			else {
				manager.setStudentName(ID, stuName);
			}
		}
		// input: <student id>, <course name>, <marks>
		else if (a.size() == 3) {
			string ID = a[0];
			string courseName = a[1];
			double grade = string2num<double>(a[2]);

			if (manager.isNewStudent(ID)) {
				Student stu(ID);
				manager.updateAllStu(ID, stu);
			}
			if (manager.isNewCourse(courseName)) {
				Course course(courseName);
				manager.updateAllCourse(courseName, course);
			}

			manager.updateStuTaken(ID, courseName, grade);
			manager.updateCourseEnroll(courseName, grade);
		}
		else {
			cout << "Input Error!" << endl;
			exit(-1);
		}
	}

	// print the first prompt line
	cout << "student id, name";
	for (map<string, Course>::iterator p = manager.all_course.begin(); p != manager.all_course.end(); p++) {
		cout << ", " << p->first;
	}
	cout << ", average" << endl;

	// print the student grades
	for (map<string, Student>::iterator p = manager.all_student.begin(); p != manager.all_student.end(); p++) {
		cout << p->first << ", " << p->second.name;
		string courseName;
		string ID = p->first;

		for (map<string, Course>::iterator q = manager.all_course.begin(); q != manager.all_course.end(); q++) {
			courseName = q->first;
			if (manager.haveTaken(ID, courseName)) {
				cout << ", " << fixed << setprecision(1) << p->second.taken[courseName];
			}
			else {
				cout << ", ";
			}
		}

		cout << ", " << fixed << setprecision(1) << p->second.average << endl;
	}

	// print the average score of each course
	double sum = 0;
	double val = 0;
	cout << ", ";
	for (map<string, Course>::iterator p = manager.all_course.begin(); p != manager.all_course.end(); p++) {
		val = p->second.totalGrade / p->second.studentNum;
		sum += val;
		cout << ", " << fixed << setprecision(1) << val;
	}
	cout << ", " << fixed << setprecision(1) << sum / manager.all_course.size() << endl;
}

string& trim(string& s)
{
	if (s.empty())
	{
		return s;
	}

	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

template <class Type>
Type string2num(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}
```



