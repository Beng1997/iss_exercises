# Simple Network Protocol

A C++ implementation of a simple network protocol with IP address and port filtering capabilities, featuring a firewall simulation system.

## Overview

This project implements a packet filtering system that:
- Parses and validates IP addresses and port numbers
- Matches packets against filtering rules
- Simulates a firewall with rule-based filtering
- Processes network packets through a command-line interface

## Architecture

The system uses object-oriented design with the following components:
- **Generic Field**: Base class for protocol fields
- **IP Class**: IP address handling and validation
- **Port Class**: Port number handling and validation
- **String Utilities**: Custom string processing
- **Firewall Script**: Bash orchestration for rule processing

## Files

### C++ Source Files
- **`main.cpp`** - Entry point, argument parsing, and rule initialization
- **`ip.cpp`** / **`ip.h`** - IP address implementation and matching logic
- **`port.cpp`** / **`port.h`** - Port number implementation and matching logic
- **`string.cpp`** - Custom string manipulation utilities
- **`generic-field.h`** - Base class interface (referenced but not shown)
- **`input.h`** - Input processing utilities (referenced but not shown)

### Scripts
- **`firewall.sh`** - Firewall simulation script for packet filtering

### Build
- **`Makefile`** - Build configuration

## How to Compile

### Using Makefile

```bash
make
```

This will generate the `firewall.exe` executable.

### Manual Compilation

```bash
g++ -Wall -Wextra -std=c++11 -o firewall.exe main.cpp ip.cpp port.cpp string.cpp
```

## How to Use

### Firewall Executable

The program accepts filtering rules in the format:

```bash
./firewall.exe "<field>=<value>"
```

#### Supported Fields:
- `src-ip` - Source IP address
- `dst-ip` - Destination IP address
- `src-port` - Source port number
- `dst-port` - Destination port number

### Rule Examples

#### Filter by Source IP
```bash
./firewall.exe "src-ip=192.168.1.1"
```

#### Filter by Destination Port
```bash
./firewall.exe "dst-port=80"
```

#### Filter by IP Range
```bash
./firewall.exe "src-ip=192.168.1.0/24"
```

#### Filter by Port Range
```bash
./firewall.exe "dst-port=8000-9000"
```

## IP Address Features

### Supported Formats

1. **Single IP Address**
   ```bash
   src-ip=192.168.1.1
   ```

2. **IP Range (CIDR Notation)**
   ```bash
   src-ip=192.168.1.0/24
   dst-ip=10.0.0.0/8
   ```

3. **IP Range (Explicit Range)**
   ```bash
   src-ip=192.168.1.1-192.168.1.254
   ```

### IP Class Structure

```cpp
class IP : public GenericField {
private:
    String pattern;        // Field name (src-ip or dst-ip)
    unsigned int min_ip;   // Minimum valid IP
    unsigned int max_ip;   // Maximum valid IP
public:
    bool match(String packet);
    bool set_value(String value);
};
```

## Port Features

### Supported Formats

1. **Single Port**
   ```bash
   src-port=80
   dst-port=443
   ```

2. **Port Range**
   ```bash
   src-port=8000-9000
   dst-port=1024-65535
   ```

3. **Any Port** (wildcard)
   ```bash
   src-port=any
   ```

## Firewall Script Usage

The `firewall.sh` script processes packet files against rule files:

```bash
./firewall.sh <rules_file> < <packets_file>
```

### Rule File Format

Rules file contains filtering criteria, one rule per line:
```
src-ip=192.168.1.0/24, dst-ip=10.0.0.1, src-port=any, dst-port=80
src-ip=172.16.0.0/16, dst-ip=any, src-port=1024-65535, dst-port=443
```

**Rule Syntax:**
- Fields separated by commas: `field1=value1, field2=value2, ...`
- Comments start with `#`
- Empty lines are ignored
- Whitespace is automatically trimmed

### Packet File Format

Packets file contains network packets, one per line:
```
src-ip=192.168.1.100, dst-ip=10.0.0.1, src-port=54321, dst-port=80
src-ip=192.168.1.200, dst-ip=10.0.0.1, src-port=54322, dst-port=443
src-ip=172.16.5.10, dst-ip=8.8.8.8, src-port=12345, dst-port=53
```

### Example Usage

Create a rules file (`rules.txt`):
```
# Allow web traffic from local network
src-ip=192.168.1.0/24, dst-ip=any, src-port=any, dst-port=80
src-ip=192.168.1.0/24, dst-ip=any, src-port=any, dst-port=443
```

Create a packets file (`packets.txt`):
```
src-ip=192.168.1.100, dst-ip=10.0.0.1, src-port=54321, dst-port=80
src-ip=192.168.1.100, dst-ip=10.0.0.1, src-port=54322, dst-port=22
src-ip=192.168.1.200, dst-ip=10.0.0.1, src-port=54323, dst-port=443
```

Run the firewall:
```bash
./firewall.sh rules.txt < packets.txt
```

**Output:** (Packets that match any rule, sorted and deduplicated)
```
src-ip=192.168.1.100,dst-ip=10.0.0.1,src-port=54321,dst-port=80
src-ip=192.168.1.200,dst-ip=10.0.0.1,src-port=54323,dst-port=443
```

## How the Firewall Works

1. **Read Rules**: Parse each rule from the rules file
2. **Initialize Packet List**: Read all packets from stdin
3. **For Each Rule**:
   - Split rule into individual field filters
   - Apply each filter sequentially to packet list
   - Keep only packets matching ALL fields
4. **Accumulate Results**: Collect packets matching any complete rule
5. **Output**: Print unique matching packets in sorted order

### Filtering Process

```
Original Packets → Filter 1 → Filter 2 → Filter 3 → Filter 4 → Matching Packets
                   (src-ip)   (dst-ip)   (src-port)  (dst-port)
```

For each rule, a packet must match ALL four field criteria to pass.

## Program Flow

```
main.cpp
    ├── check_args() → Validate command line arguments
    ├── String.split() → Parse rule into field=value
    ├── IP or Port object → Create appropriate field object
    │   ├── set_value() → Parse and validate value
    │   └── parse_input() → Read and match packets
    └── Return status
```

## String Class Features

The custom `String` class provides:
- String splitting by delimiter
- Trimming whitespace
- Equality comparison
- Memory management

```cpp
String argument(argv[1]);
String *substrings;
size_t size;
argument.split("=", &substrings, &size);
```

## Technical Details

### IP Address Representation
- IPs stored as unsigned 32-bit integers internally
- Range checking for CIDR notation
- Bitwise operations for subnet matching

### Port Number Validation
- Valid range: 0-65535
- Special handling for "any" keyword
- Range validation for min/max bounds

### Error Handling
- Returns `-1` (ERROR) for invalid input
- Validates rule format (field=value)
- Checks field type (src-ip, dst-ip, src-port, dst-port)
- Validates value format for each field type

## Building Tips

### Required Compiler Features
- C++11 or later
- Standard Template Library (STL)

### Compilation Flags
```bash
-Wall          # Enable all warnings
-Wextra        # Extra warnings
-std=c++11     # C++11 standard
-o firewall.exe # Output executable name
```

## Advanced Features

### Generic Field Pattern
The `GenericField` base class enables:
- Polymorphism for different field types
- Consistent interface for matching operations
- Extensibility for new field types

### Rule Composition
- Multiple rules create OR logic (packet matches ANY rule)
- Multiple fields within a rule create AND logic (packet matches ALL fields)

### Performance Optimizations
- Packet list filtering is sequential
- Early termination when no packets remain
- Deduplication using Unix `sort | uniq`

## Common Use Cases

### 1. Allow HTTP/HTTPS Traffic
```bash
# Rules
src-ip=any, dst-ip=any, src-port=any, dst-port=80
src-ip=any, dst-ip=any, src-port=any, dst-port=443
```

### 2. Block Traffic from Subnet
```bash
# Allow all except 192.168.1.0/24
src-ip=0.0.0.0-192.168.0.255, dst-ip=any, src-port=any, dst-port=any
src-ip=192.168.2.0-255.255.255.255, dst-ip=any, src-port=any, dst-port=any
```

### 3. Allow SSH from Specific IP
```bash
src-ip=192.168.1.100, dst-ip=any, src-port=any, dst-port=22
```

## Limitations

- Single-threaded processing
- In-memory packet storage (large packet files may consume significant memory)
- No stateful filtering (each packet evaluated independently)
- Rule syntax is strict (spaces matter in some contexts)

## Dependencies

- C++ Standard Library
- Bash shell (for firewall script)
- Standard Unix utilities: `tr`, `sed`, `grep`, `cut`, `sort`, `uniq`

## Exit Codes

- `0` - Success, packet matches rule
- `-1` (ERROR) - Invalid input or format error

## Notes

- The firewall script removes all spaces and tabs from input for consistent parsing
- Comments in rule files must start with `#`
- Packets are deduplicated in final output
- Empty lines in rule files are automatically skipped
- The script uses process substitution for efficient filtering
