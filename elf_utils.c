// Sample optional helper file for handling ELF parsing
#include <elf.h>
#include <stdio.h>

// Helper function to identify ELF sections (can be expanded as needed)
void print_section_names(void *map, Elf64_Ehdr *ehdr) {
    Elf64_Shdr *shdr = (Elf64_Shdr *)(map + ehdr->e_shoff);
    Elf64_Shdr *strtab = &shdr[ehdr->e_shstrndx];
    char *shstrtab = (char *)(map + strtab->sh_offset);

    for (int i = 0; i < ehdr->e_shnum; i++) {
        printf("Section %d: %s\n", i, &shstrtab[shdr[i].sh_name]);
    }
}
