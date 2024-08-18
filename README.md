# UTF8-To-Varint-Varint-To-UTF8

This code provides functions to convert between UTF-8 and Varint encoding formats. Below is a brief description of the code:

1. **Byte Counting Functions**:
   - `contaBytes_UTF8`: Determines the number of bytes in a UTF-8 character based on its leading byte.
   - `contaBytes_Varint`: Determines the number of bytes required for a Varint based on its value.

2. **UTF-8 to Varint Conversion**:
   - `utf8tovarint2`, `utf8tovarint3`, `utf8tovarint4`: These helper functions convert UTF-8 encoded values of 2, 3, and 4 bytes, respectively, into Varint format. The conversion involves bitwise operations to rearrange the bits from UTF-8 format into the more compact Varint format.

3. **Varint to UTF-8 Conversion**:
   - `varinttoutf`: Converts a Varint value back into its corresponding UTF-8 encoding. The function handles the conversion based on the number of bytes in the Varint.

4. **File Conversion Functions**:
   - `varint2utf`: Reads a Varint-encoded file and writes its UTF-8 equivalent to an output file. The function processes the input byte by byte, converting Varint sequences to UTF-8 characters as it reads them.
   - `utf2varint`: Reads a UTF-8 encoded file and writes its Varint equivalent to an output file. The function identifies UTF-8 sequences and converts them into the corresponding Varint format.

The code effectively handles the intricacies of UTF-8 and Varint formats by using bitwise operations to manipulate and convert data between these two encodings.
