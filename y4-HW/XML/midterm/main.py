import xml.etree.ElementTree as ET
import mysql.connector

db_base_config = {
    'host': 'localhost',
    'user': 'root',
    'password': ''
}

DB_NAME = 'xmldb'

# ===========================================================
# Pre: Auto build Schema, Table
# ===========================================================
def init_database():
    print("--- Initialize the database and tables ---")
    conn = mysql.connector.connect(**db_base_config)
    cursor = conn.cursor()
    cursor.execute(f"CREATE DATABASE IF NOT EXISTS {DB_NAME};")
    cursor.execute(f"USE {DB_NAME};")
    create_table_query = """
        CREATE TABLE IF NOT EXISTS score_data (
            student_id VARCHAR(5) PRIMARY KEY NOT NULL,
            xml_score INT,
            data_structure_score INT,
            algorithm_score INT,
            network_score INT
        );
    """
    cursor.execute(create_table_query)
    conn.commit()
    cursor.close()
    print("The database and Tables are ready!")

def get_db_connection():
    config = db_base_config.copy()
    config['database'] = DB_NAME
    return mysql.connector.connect(**config)
    
# ===========================================================
# Part 1: XML Parsing and Database Persistence
# ===========================================================
def part1(xml_file_path):
    print("--- Execute Part 1: Parse XML and write it to the database ---")
    tree = ET.parse(xml_file_path)
    root = tree.getroot()
    conn = get_db_connection()
    cursor = conn.cursor()
    insert_query = """
        INSERT INTO score_data (student_id, xml_score, data_structure_score, algorithm_score, network_score)
        VALUES (%s, %s, %s, %s, %s)
        ON DUPLICATE KEY UPDATE
            xml_score = VALUES(xml_score),
            data_structure_score = VALUES(data_structure_score),
            algorithm_score = VALUES(algorithm_score),
            network_score = VALUES(network_score);
    """
    for student in root.findall('student'):
        student_id = student.attrib['student_id']
        xml_score = int(student.find('xml_class').text)
        ds_score = int(student.find('data_structure').text)
        algo_score = int(student.find('algorithm').text)
        net_score = int(student.find('network').text)
        cursor.execute(insert_query, (student_id, xml_score, ds_score, algo_score, net_score))
    conn.commit()
    cursor.close()
    conn.close()
    print("Part 1 Complete: Data has been successfully written to MySQL!")

# ===========================================================
# Part 2: Data Retrieval, GPA Calculation, and XML Generation
# ===========================================================
def get_gpa(score):
    if score >= 90: return "4.5"
    elif score >= 85: return "4"
    elif score >= 80: return "3.7"
    elif score >= 77: return "3.3"
    elif score >= 73: return "3.0"
    elif score >= 70: return "2.7"
    elif score >= 67: return "2.5"
    elif score >= 63: return "2.3"
    elif score >= 60: return "2"
    elif score >= 50: return "1"
    else: return "0.0"

def part2(output_xml_path):
    print("--- Execute Part 2: Data Retrieval, GPA Calculation, and XML Generation ---")
    conn = get_db_connection()
    cursor = conn.cursor(dictionary=True)
    cursor.execute("SELECT * FROM score_data")
    rows = cursor.fetchall()
    root = ET.Element("score_data")
    for row in rows:
        student_elem = ET.SubElement(root, "student", student_id=row['student_id'])
        xml_elem = ET.SubElement(student_elem, "xml_class", gpa=get_gpa(row['xml_score']))
        xml_elem.text = str(row['xml_score'])
        
        ds_elem = ET.SubElement(student_elem, "data_structure", gpa=get_gpa(row['data_structure_score']))
        ds_elem.text = str(row['data_structure_score'])
        
        algo_elem = ET.SubElement(student_elem, "algorithm", gpa=get_gpa(row['algorithm_score']))
        algo_elem.text = str(row['algorithm_score'])
        
        net_elem = ET.SubElement(student_elem, "network", gpa=get_gpa(row['network_score']))
        net_elem.text = str(row['network_score'])
    cursor.close()
    conn.close()
    tree = ET.ElementTree(root)
    ET.indent(tree, space="    ")
    tree.write(output_xml_path, encoding="utf-8", xml_declaration=True)
    print(f"Part 2 Complete: Successfully exported to {output_xml_path}!")

if __name__ == "__main__":
    init_database()
    part1("score_data.xml")
    part2("output.xml")