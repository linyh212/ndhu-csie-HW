#!/usr/bin/env python3

import argparse
from pathlib import Path
from lxml import etree

def validate(xml_path: str, xsd_path: str) -> bool:
    xml_file = Path(xml_path)
    xsd_file = Path(xsd_path)
    
    if not xml_file.exists():
        print(f"[ERROR] XML file not found: {xml_file}")
        return False
    if not xsd_file.exists():
        print(f"[ERROR] XSD file not found: {xsd_file}")
        return False

    try:
        xsd_doc = etree.parse(str(xsd_file))
        schema = etree.XMLSchema(xsd_doc)
    except (etree.XMLSchemaParseError, etree.XMLSyntaxError, OSError) as exc:
        print(f"[ERROR] Could not load XSD schema: {xsd_file}")
        if hasattr(exc, "error_log") and len(exc.error_log) > 0:
            for error in exc.error_log:
                print(f"  Line {error.line}, column {error.column}, {error.message}")
        else:
            print(f"  {exc}")
        return False

    try:
        parser = etree.XMLParser(no_network=True)
        xml_doc = etree.parse(str(xml_file), parser)
    except (etree.XMLSyntaxError, OSError) as exc:
        print(f"[INVALID] XML parsing failed: {xml_file}")
        if hasattr(exc, "error_log") and len(exc.error_log) > 0:
            for error in exc.error_log:
                print(f"  Line {error.line}, column {error.column}, {error.message}")
        else:
            print(f"  {exc}")
        return False

    if schema.validate(xml_doc):
        print(f"[VALID] {xml_file.name} conforms to {xsd_file.name}.")
        return True
    
    print(f"[INVALID] {xml_file.name} does not conform to {xsd_file.name}.")
    for error in schema.error_log:
        print(f"  Line {error.line}, column {error.column}, {error.message}")
    return False

def main():
    parser = argparse.ArgumentParser(
        description="Validate an XML document against an external XSD schema."
    )
    parser.add_argument(
        "xml",
        help="Path to the XML file"
    )
    parser.add_argument(
        "xsd",
        help="Path to the XSD file"
    )
    args = parser.parse_args()
    result = validate(
        args.xml,
        args.xsd
    )
    raise SystemExit(
        0 if result else 1
    )

if __name__ == "__main__":
    main()