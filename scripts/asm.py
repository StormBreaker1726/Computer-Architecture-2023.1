import re
import sys
from  argparse import ArgumentParser

PROG_NAME = "asm"

REG_NUM_REGEX         = r"(\d|[1-2]\d|3[0-1])"
REG_NUM_NO_ZERO_REGEX = r"([1-9]|[1-2]\d|3[0-1])"

REG_REGEX             = fr"(x|f){REG_NUM_REGEX}"
REG_NO_ZERO_REGEX     = fr"(x{REG_NUM_NO_ZERO_REGEX}|f{REG_NUM_REGEX})"

# =================== INSTRUCTIONS ARGUMENTS REGEX =================== #
FLD_ARGS_REGEX    = fr"{REG_NO_ZERO_REGEX},\s*{REG_REGEX},\s*\d+"
FSD_ARGS_REGEX    = fr"{REG_NO_ZERO_REGEX},\s*\d+\s*\({REG_REGEX}\)"
FADD_D_ARGS_REGEX = fr"{REG_NO_ZERO_REGEX},\s*{REG_REGEX},\s*{REG_REGEX}"
FSUB_D_ARGS_REGEX = fr"{REG_NO_ZERO_REGEX},\s*{REG_REGEX},\s*{REG_REGEX}"
FMUL_D_ARGS_REGEX = fr"{REG_NO_ZERO_REGEX},\s*{REG_REGEX},\s*{REG_REGEX}"
FDIV_D_ARGS_REGEX = fr"{REG_NO_ZERO_REGEX},\s*{REG_REGEX},\s*{REG_REGEX}"

# =================== REGISTERS =================== #
REGISTERS = {
    "x0" : 0 , "x1" : 1 , "x2" : 2 , "x3" : 3 ,
    "x4" : 4 , "x5" : 5 , "x6" : 6 , "x7" : 7 ,
    "x8" : 8 , "x9" : 9 , "x10": 10, "x11": 11,
    "x12": 12, "x13": 13, "x14": 14, "x15": 15,
    "x16": 16, "x17": 17, "x18": 18, "x19": 19,
    "x20": 20, "x21": 21, "x22": 22, "x23": 23,
    "x24": 24, "x25": 25, "x26": 26, "x27": 27,
    "x28": 28, "x29": 29, "x30": 30, "x31": 31,
    "f0" : 32, "f1" : 33, "f2" : 34, "f3" : 35,
    "f4" : 36, "f5" : 37, "f6" : 38, "f7" : 39,
    "f8" : 40, "f9" : 41, "f10": 42, "f11": 43,
    "f12": 44, "f13": 45, "f14": 46, "f15": 47,
    "f16": 48, "f17": 49, "f18": 50, "f19": 51,
    "f20": 52, "f21": 53, "f22": 54, "f23": 55,
    "f24": 56, "f25": 57, "f26": 58, "f27": 59,
    "f28": 60, "f29": 61, "f30": 62, "f31": 63
}

# =================== ARGUMENT SPLIT =================== #
def comma_split(arguments):
    return map(lambda arg: arg.strip(), arguments.split(','))

def fsd_split_fn(arguments):
    match = re.search(r"\s*(\S*?),\s*(\d+)\s*\((.*?)\)", arguments)
    if match:
        return match.groups()

# =================== INSTRUCTIONS HELPERS =================== #
def reg_name_to_reg_num(reg_name: str):
    reg_num = REGISTERS[reg_name]
    return reg_num

def to_s_imm(imm: int):
    # Divided in two parts:
    # Length 7 (31-25)
    imm_11_5 = (imm & 0x7F) << 25
    # Length 5 (11-7)
    imm_4_0 = imm & 0x1F << 7
    return imm_11_5 | imm_4_0

def to_i_imm(imm: int):
    # Length: 12 (31-20)
    return (imm & 0xFFF) << 20

def to_rs1(rs1: int):
    # Length: 5 (19-15)
    return (rs1 & 0x1F) << 15

def to_rs2(rs2: int):
    # Length: 5 (24-20)
    return (rs2 & 0x1F) << 20

def to_funct7(funct7: int):
    # Length 7 (31-25)
    return (funct7 & 0x7F) << 25

def to_funct3(funct3: int):
    # Length: 3 (14-12)
    return (funct3 & 0x7) << 12

def to_rd(rd: int):
    # Length: 5 (11-7)
    return (rd & 0x1F) << 7

def to_opcode(opcode: int):
    # Length: 7 (6 - 0)
    return opcode & 0x7F

def r_instruction(funct7, rs2, rs1, funct3, rd, opcode):
    return to_funct7(funct7) | to_rs2(rs2) | to_rs1(rs1) | to_funct3(funct3) | to_rd(rd) | to_opcode(opcode)

def i_instruction(imm, rs1, funct3, rd, opcode):
    return to_i_imm(imm) | to_rs1(rs1) | to_funct3(funct3) | to_rd(rd) | to_opcode(opcode)

def s_instruction(imm, rs2, rs1, funct3, opcode):
    return to_s_imm(imm) | to_rs2(rs2) | to_rs1(rs1) | to_funct3(funct3) | to_opcode(opcode)

def fld_bin(rd, rs1, offset):
    rd     = reg_name_to_reg_num(rd)
    rs1    = reg_name_to_reg_num(rs1)
    offset = int(offset)
    return i_instruction(offset, rs1, 0x3, rd, 0x7)

def fsd_bin(rs2, offset, rs1):
    rs2    = reg_name_to_reg_num(rs2)
    offset = int(offset)
    rs1    = reg_name_to_reg_num(rs1)
    return s_instruction(offset, rs2, rs1, 0x3, 0x27)

def fadd_d_bin(rd, rs1, rs2):
    rd  = reg_name_to_reg_num(rd)
    rs1 = reg_name_to_reg_num(rs1)
    rs2 = reg_name_to_reg_num(rs2)
    return r_instruction(0x1, rs2, rs1, 0, rd, 0x53)

def fsub_d_bin(rd, rs1, rs2):
    rd  = reg_name_to_reg_num(rd)
    rs1 = reg_name_to_reg_num(rs1)
    rs2 = reg_name_to_reg_num(rs2)
    return r_instruction(0x5, rs2, rs1, 0, rd, 0x53)

def fmul_d_bin(rd, rs1, rs2):
    rd  = reg_name_to_reg_num(rd)
    rs1 = reg_name_to_reg_num(rs1)
    rs2 = reg_name_to_reg_num(rs2)
    return r_instruction(0x9, rs2, rs1, 0, rd, 0x53)

def fdiv_d_bin(rd, rs1, rs2):
    rd  = reg_name_to_reg_num(rd)
    rs1 = reg_name_to_reg_num(rs1)
    rs2 = reg_name_to_reg_num(rs2)
    return r_instruction(0xD, rs2, rs1, 0, rd, 0x53)

def make_instruction(instruction: int | str, comment: str | None):
    if type(instruction) == int:
        instruction = f"{instruction:032b}"
    if comment and len(comment) > 1:
        instruction = f"# {comment}\n{instruction}"
    return instruction


# =================== INSTRUCTIONS =================== #
def fld(arguments):
    [rd, rs1, offset] = arguments
    comment = f"fld {rd}, {rs1}, {offset}"
    instruction_bin = fld_bin(rd, rs1, offset)
    return make_instruction(instruction_bin, comment)

def fsd(arguments):
    [rs2, offset, rs1] = arguments
    comment = f"fsd {rs2}, {offset}({rs1})"
    instruction_bin = fsd_bin(rs2, offset, rs1)
    return make_instruction(instruction_bin, comment)

def fadd_d(arguments):
    [rd, rs1, rs2] = arguments
    comment = f"fadd.d {rd}, {rs1}, {rs2}"
    instruction_bin = fadd_d_bin(rd, rs1, rs2)
    return make_instruction(instruction_bin, comment)

def fsub_d(arguments):
    [rd, rs1, rs2] = arguments
    comment = f"fsub.d {rd}, {rs1}, {rs2}"
    instruction_bin = fsub_d_bin(rd, rs1, rs2)
    return make_instruction(instruction_bin, comment)

def fmul_d(arguments):
    [rd, rs1, rs2] = arguments
    comment = f"fmul.d {rd}, {rs1}, {rs2}"
    instruction_bin = fmul_d_bin(rd, rs1, rs2)
    return make_instruction(instruction_bin, comment)

def fdiv_d(arguments):
    [rd, rs1, rs2]  = arguments
    instruction_bin = fdiv_d_bin(rd, rs1, rs2)
    comment         = f"fdiv.d {rd}, {rs1}, {rs2}"
    return make_instruction(instruction_bin, comment)

# ===================================================== #
def get_or_throw(map, key):
    value = map.get(key)
    if value is None:
        raise TypeError(f"Field `{key}` is not defined")
    return value

def instruction_info_get_argument_regex(instruction_info):
    return get_or_throw(instruction_info, "argument-regex")

def instruction_info_get_argument_split(instruction_info):
    return get_or_throw(instruction_info, "argument-split")

def instruction_info_get_to_bin(instruction_info):
    return get_or_throw(instruction_info, "to-bin")

def asm(input_file, output_file):
    instructions_info_table = {
        "fld": {
            "argument-regex": FLD_ARGS_REGEX,
            "argument-split": comma_split,
            "to-bin": fld
        },
        "fsd": {
            "argument-regex": FSD_ARGS_REGEX,
            "argument-split": fsd_split_fn,
            "to-bin": fsd,
        },
        "fadd.d": {
            "argument-regex": FADD_D_ARGS_REGEX,
            "argument-split": comma_split,
            "to-bin": fadd_d,
        },
        "fsub.d": {
            "argument-regex": FSUB_D_ARGS_REGEX,
            "argument-split": comma_split,
            "to-bin": fsub_d,
        },
        "fmul.d": {
            "argument-regex": FMUL_D_ARGS_REGEX,
            "argument-split": comma_split,
            "to-bin": fmul_d,
        },
        "fdiv.d": {
            "argument-regex": FDIV_D_ARGS_REGEX,
            "argument-split": comma_split,
            "to-bin": fdiv_d,
        },
    }

    linenum = 0
    for line in input_file:
        linenum += 1
        splits = line.strip().split(maxsplit=1)

        if len(splits) == 0:
            continue
        if len(splits) < 2:
            raise RuntimeError(f"Invalid syntax at line {linenum}\n`{line}`")
        else:
            [instruction, arguments] = splits;

            instruction_info = instructions_info_table.get(instruction)
            if instruction_info is None:
                raise RuntimeError(f"Invalid instruction: `{instruction}` at line {linenum}")

            arg_regex = instruction_info_get_argument_regex(instruction_info)
            arg_match = re.fullmatch(arg_regex, arguments)
            if not arg_match:
                raise RuntimeError(f"Invalid arguments for instruction `{instruction}` (arguments: `{arguments}`, line: {linenum})")

            arg_split = instruction_info_get_argument_split(instruction_info)
            arguments = arg_split(arguments)

            to_bin = instruction_info_get_to_bin(instruction_info)

            print(to_bin(arguments), file=output_file)

def main():
    arg_parser = ArgumentParser(prog=PROG_NAME, description="RISC-V mini compiler")
    arg_parser.add_argument('-i', '--ifile')
    arg_parser.add_argument('-o', '--ofile')

    args = arg_parser.parse_args()

    input_file  = sys.stdin
    output_file = sys.stdout

    if args.ifile:
        input_file = open(args.ifile, 'r')
    if args.ofile:
        output_file = open(args.ofile, 'w')

    asm(input_file, output_file)

    if input_file != sys.stdin:
        input_file.close()
    if output_file != sys.stdout:
        output_file.close()

if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"{PROG_NAME}: {e}", file=sys.stderr)
        exit(1)
