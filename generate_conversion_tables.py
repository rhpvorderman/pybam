def character_to_bam_op_table():
    table = [str(-1) for _ in range(256)]
    table[ord("M")] = "BAM_CMATCH"
    table[ord("I")] = "BAM_CINS"
    table[ord("D")] = "BAM_CDEL"
    table[ord("N")] = "BAM_CREF_SKIP"
    table[ord("S")] = "BAM_CSOFT_CLIP"
    table[ord("H")] = "BAM_CHARD_CLIP"
    table[ord("P")] = "BAM_CPAD"
    table[ord("=")] = "BAM_CEQUAL"
    table[ord("X")] = "BAM_CDIFF"
    table[ord("B")] = "BAM_CBACK"
    return table


def main():
    with open("src/pybam/conversions.h", "wt") as out:
        out.write('#include "htslib/sam.h"\n')
        out.write('\n')
        out.write('static const bam_cigar_table[256] = {')
        for i, literal in enumerate(character_to_bam_op_table()):
            if i % 8 == 0:
                out.write("\n    ")
            out.write(literal.rjust(14, " ") + ", ")
        out.write("\n")
        out.write("}\n")


if __name__ == "__main__":
    main()