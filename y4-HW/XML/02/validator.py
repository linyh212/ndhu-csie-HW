#!/usr/bin/env python3

import argparse
from pathlib import Path
from lxml import etree

def validate(xml_path: str, dtd_path: str) -> bool:
    xml_file = Path(xml_path)
    dtd_file = Path(dtd_path)
    if not xml_file.exists():
            print(f"[ERROR] XML file not found: {xml_file}")
            return False
    if not dtd_file.exists():
        print(f"[ERROR] DTD file not found: {dtd_file}")
        return False
    
    try:
        parser = etree.XMLParser(
            load_dtd = True,
            no_network = True
        )
        tree = etree.parse(str(xml_file), parser)
    except (etree.XMLSyntaxError, OSError) as exc:
        print(f"[INVALID] XML parsing failed: {xml_file}")
        if hasattr(exc, "error_log") and len(exc.error_log) > 0:
            for error in exc.error_log:
                print(
                    f"  Line {error.line}, "
                    f"column {error.column}, "
                    f"{error.message}"
                )
        else:
            print(f"  {exec}")
        return False
    try:
        dtd = etree.DTD(str(dtd_file))
    except (etree.DTDParseError, OSError) as exc:
        print(f"[ERROR] Could not load DTD: {dtd_file}")
        print(f"  {exc}")
        return False
    
    if dtd.validate(tree):
        print(
            f"[VALID] {xml_file.name} "
            f"conforms to {dtd_file.name}."
        )
        return True
    print(
        f"[INVALID] {xml_file.name} "
        f"does not conform to {dtd_file.name}."
    )
    for error in dtd.error_log:
        print(
            f"  Line {error.line}, "
            f"column {error.column}, "
            f"{error.message}"
        )
    return False

def main():
    parser = argparse.ArgumentParser(
        description="Validate an XML document against an external DTD."
    )
    parser.add_argument(
        "xml",
        help="Path to the XML file"
    )
    parser.add_argument(
        "dtd",
        help="Path to the DTD file"
    )
    args = parser.parse_args()
    result = validate(
        args.xml,
        args.dtd
    )
    raise SystemExit(
        0 if result else 1
    )

if __name__ == "__main__":
    main()