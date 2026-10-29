===============================
Mid-Term Assignment - 411221420
===============================

1. VIDEO PRESENTATION DEMO LINK
-------------------------------
Video Link: https://youtu.be/44gHhciAZI8

2. OVERVIEW
-------------------------------
This project handles XML parsing, database persistence, GPA calculation, and output XML generation using Python and MySQL.
- Part 1: Parses `score_data.xml` and imports the student grades into a MySQL database under schema `xmldb` and table `score_data`.
- Part 2: Queries `score_data` from MySQL, converts numeric grades into Grade Points (GP) based on the grading scale, and exports the data with `gpa` attributes into `output.xml`.

3. ENVIRONMENT & PREREQUISITES
-------------------------------
- Python 3.8+
- MySQL Server & MySQL Workbench
- Required Python Packages:
  * mysql-connector-python

To install required dependencies:
  pip install -r requirements.txt

4. DATABASE SETUP
-------------------------------
The script `main.py` automatically initializes the database `xmldb` and the table `score_data` upon execution. 
Ensure MySQL service is running and update the database 
credentials in `main.py` if needed:

db_base_config = {
    'host': 'localhost',
    'user': 'root',
    'password': 'your_password'
}

5. HOW TO RUN THE CODE
-------------------------------
a. Place `score_data.xml` in the same directory as `main.py`.
b. Run the main execution script:
   python3 main.python
c. Output Verification:
   - Check MySQL Workbench: Database `xmldb` -> Table `score_data`.
   - Check local directory for the generated `output.xml`.
