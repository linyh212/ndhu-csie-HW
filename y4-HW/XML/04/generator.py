import sys
from pathlib import Path
from collections import Counter
from lxml import etree

XS = "http://www.w3.org/2001/XMLSchema"
NSMAP = {"xs": XS}

def infer_type(values):
    values = [v.strip() for v in values if v is not None and v.strip()]
    if not values:
        return "xs:string"
    lower_values = [v.lower() for v in values]
    if all(v in ("true", "false", "1", "0") for v in lower_values):
        if any(v in ("true", "false") for v in lower_values):
            return "xs:boolean"
    try:
        for value in values:
            int(value)
        return "xs:integer"
    except ValueError:
        pass
    try:
        for value in values:
            float(value)
        if any("." in value or "e" in value.lower() for value in values):
            return "xs:decimal"
        return "xs:integer"
    except ValueError:
        pass
    return "xs:string"

def collect_values(elements):
    return [
        element.text.strip()
        for element in elements
        if element.text and element.text.strip()
    ]

def build_element(parent, elements, global_root):
    name = elements[0].tag
    declaration = etree.SubElement(parent, f"{{{XS}}}element", name=name)
    child_elements = [
        child
        for element in elements
        for child in element
        if isinstance(child.tag, str)
    ]
    if not child_elements:
        values = collect_values(elements)
        declaration.set("type", infer_type(values))
        return

    complex_type = etree.SubElement(
        declaration, f"{{{XS}}}complexType"
    )
    sequence = etree.SubElement(
        complex_type, f"{{{XS}}}sequence"
    )
    child_names = []
    for element in elements:
        for child in element:
            if isinstance(child.tag, str) and child.tag not in child_names:
                child_names.append(child.tag)
    for child_name in child_names:
        samples = [
            child
            for element in elements
            for child in element
            if isinstance(child.tag, str) and child.tag == child_name
        ]
        build_element(sequence, samples, global_root)
        child_declaration = sequence[-1]
        counts = [
            sum(
                1 for child in element
                if isinstance(child.tag, str) and child.tag == child_name
            )
            for element in elements
        ]
        if max(counts, default=0) > 1:
            child_declaration.set("maxOccurs", "unbounded")
    attribute_names = []
    for element in elements:
        for attribute_name in element.attrib:
            if attribute_name not in attribute_names:
                attribute_names.append(attribute_name)
    for attribute_name in attribute_names:
        values = [
            element.get(attribute_name)
            for element in elements
            if element.get(attribute_name) is not None
        ]
        if attribute_name == "id":
            attribute_type = "xs:ID"
        else:
            attribute_type = infer_type(values)
        etree.SubElement(
            complex_type,
            f"{{{XS}}}attribute",
            name=attribute_name,
            type=attribute_type,
            use="required"
        )

def generate_schema(xml_path, xsd_path):
    xml_path = Path(xml_path)
    xsd_path = Path(xsd_path)

    if not xml_path.is_file():
        print(f"[ERROR] XML file not found: {xml_path}")
        return False
    try:
        parser = etree.XMLParser(
            resolve_entities=False,
            no_network=True
        )
        xml_tree = etree.parse(str(xml_path), parser)
    except (etree.XMLSyntaxError, OSError) as error:
        print("[ERROR] Cannot parse the input XML.")
        print(error)
        return False
    xml_root = xml_tree.getroot()
    schema_root = etree.Element(
        f"{{{XS}}}schema",
        nsmap={"xs": XS}
    )
    root_declaration = etree.SubElement(
        schema_root,
        f"{{{XS}}}element",
        name=xml_root.tag
    )
    root_complex = etree.SubElement(
        root_declaration,
        f"{{{XS}}}complexType"
    )
    root_sequence = etree.SubElement(
        root_complex,
        f"{{{XS}}}sequence"
    )
    child_names = []
    for child in xml_root:
        if isinstance(child.tag, str) and child.tag not in child_names:
            child_names.append(child.tag)
    for child_name in child_names:
        samples = [
            child for child in xml_root
            if isinstance(child.tag, str) and child.tag == child_name
        ]
        build_element(root_sequence, samples, schema_root)
        child_declaration = root_sequence[-1]
        counts = [
            sum(
                1 for child in xml_root
                if isinstance(child.tag, str) and child.tag == child_name
            )
        ]
        if max(counts, default=0) > 1:
            child_declaration.set("maxOccurs", "unbounded")
    try:
        xsd_tree = etree.ElementTree(schema_root)
        xsd_tree.write(
            str(xsd_path),
            encoding="UTF-8",
            xml_declaration=True,
            pretty_print=True
        )
    except OSError as error:
        print("[ERROR] Cannot write the generated XSD.")
        print(error)
        return False
    print(f"[SUCCESS] Generated XML Schema: {xsd_path}")
    print(f"[INFO] Root element: {xml_root.tag}")
    print(f"[INFO] Output file: {xsd_path.resolve()}")
    return True

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 generator.py <input.xml> <output.xsd>")
        sys.exit(1)
    success = generate_schema(sys.argv[1], sys.argv[2])
    sys.exit(0 if success else 1)