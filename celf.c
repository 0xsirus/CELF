/*
 *  By Cyrus Sh ~cyn  
 *  sirus.shahini@gmail.com
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



unsigned char buf [1000000]; //1M max

struct _8_bytes{
    unsigned char data[8];
};
struct _4_bytes{
    unsigned char data[4];
};
struct _2_bytes{
    unsigned char data[2];
};
unsigned char _byte;

typedef unsigned long int   Elf64_Addr;   //8
typedef unsigned long int   Elf64_Off;    //8
typedef unsigned short int  Elf64_Half;   //2
typedef unsigned int        Elf64_Word;   //4
typedef int                 Elf64_Sword;  //4
typedef unsigned long int   Elf64_Xword;  //8
typedef long int            Elf64_Sxword; //8

typedef struct
{
    unsigned char           e_ident[16];        
    Elf64_Half              e_type;             
    Elf64_Half              e_machine;          
    Elf64_Word              e_version;          
    Elf64_Addr              e_entry;            
    Elf64_Off               e_phoff;            
    Elf64_Off               e_shoff;            
    Elf64_Word              e_flags;            
    Elf64_Half              e_ehsize;           
    Elf64_Half              e_phentsize;        
    Elf64_Half              e_phnum;            
    Elf64_Half              e_shentsize;        
    Elf64_Half              e_shnum;            
    Elf64_Half              e_shstrndx;         
} Elf64_Ehdr;

typedef struct
{
    Elf64_Word              sh_name;            
    Elf64_Word              sh_type;            
    Elf64_Xword             sh_flags;           
    Elf64_Addr              sh_addr;            
    Elf64_Off               sh_offset;          
    Elf64_Xword             sh_size;            
    Elf64_Word              sh_link;            
    Elf64_Word              sh_info;            
    Elf64_Xword             sh_addralign;       
    Elf64_Xword             sh_entsize;         
} Elf64_Shdr;   //section header entry
typedef struct
{
    Elf64_Addr              r_offset;           /* Address of reference */
    Elf64_Xword             r_info;             /* Symbol index and type of relocation */
} Elf64_Rel_entry;
typedef struct
{
    Elf64_Addr              r_offset;           /* Address of reference */
    Elf64_Xword             r_info;             /* Symbol index and type of relocation */
    Elf64_Sxword            r_addend;           /* Constant part of expression */
} Elf64_Rela_entry;
typedef struct  //for symbol tables (.dynsym and .dynstr)
{
    Elf64_Word              st_name;
    unsigned char           st_info;
    unsigned char           st_other;
    Elf64_Half              st_shndx;
    Elf64_Addr              st_value;
    Elf64_Xword             st_size;
} Elf64_Sym_entry;
typedef struct { //for .dynamic section -> PT_DYNAMIC header
	Elf64_Sxword	d_tag;
   	union {
   		Elf64_Xword	d_val;
   		Elf64_Addr	d_ptr;
	} d_un;
} Elf64_Dyn_entry;

typedef struct {    //Program header holds only an array of these entries
	Elf64_Word	p_type;
	Elf64_Word	p_flags;
	Elf64_Off	p_offset;
	Elf64_Addr	p_vaddr;
	Elf64_Addr	p_paddr;
	Elf64_Xword	p_filesz;
	Elf64_Xword	p_memsz;
	Elf64_Xword	p_align;
} Elf64_Phdr_entry;


#define ELF64_R_SYM(i)    ((i)>>32)
#define ELF64_R_TYPE(i)   ((i)&0xffffffffL)
#define ELF64_R_INFO(s,t) (((s)<<32)+((t)&0xffffffffL))



#define SHT_NULL            0 
#define SHT_PROGBITS        1 
#define SHT_SYMTAB          2 
#define SHT_STRTAB          3 
#define SHT_RELA            4 
#define SHT_HASH            5 
#define SHT_DYNAMIC         6 
#define SHT_NOTE            7 
#define SHT_NOBITS          8 
#define SHT_REL             9 
#define SHT_SHLIB           10 
#define SHT_DYNSYM          11 
#define SHT_INIT_ARRAY      14
#define SHT_FINI_ARRAY      15
#define SHT_PREINIT_ARRAY   16
#define SHT_GROUP           17
#define SHT_SYMTAB_SHNDX    18      
#define SHT_LOOS            0x60000000 
#define SHT_HIOS            0x6FFFFFFF 
#define SHT_LOPROC          0x70000000 
#define SHT_HIPROC          0x7FFFFFFF
#define SHT_LOUSER          0x80000000
#define SHT_HIUSER          0xffffffff


#define PT_NULL 	        0
#define PT_LOAD 	        1
#define PT_DYNAMIC 	        2
#define PT_INTERP 	        3
#define PT_NOTE 	        4
#define PT_SHLIB 	        5
#define PT_PHDR 	        6
#define PT_TLS 	            7
#define PT_LOOS 	        0x60000000
#define PT_HIOS 	        0x6fffffff
#define PT_LOPROC 	        0x70000000
#define PT_HIPROC 	        0x7fffffff

#define DT_NULL 	        0 	//ignored 	mandatory 	mandatory
#define DT_NEEDED 	        1 	//d_val 	optional 	optional
#define DT_PLTRELSZ 	    2 	//d_val 	optional 	optional
#define DT_PLTGOT 	        3 	//d_ptr 	optional 	optional
#define DT_HASH 	        4 	//d_ptr 	mandatory 	mandatory
#define DT_STRTAB 	        5 	//d_ptr 	mandatory 	mandatory
#define DT_SYMTAB 	        6 	//d_ptr 	mandatory 	mandatory
#define DT_RELA 	        7 	//d_ptr 	mandatory 	optional
#define DT_RELASZ 	        8 	//d_val 	mandatory 	optional
#define DT_RELAENT 	        9 	//d_val 	mandatory 	optional
#define DT_STRSZ 	        10 	//d_val 	mandatory 	mandatory
#define DT_SYMENT 	        11 	//d_val 	mandatory 	mandatory
#define DT_INIT 	        12 	//d_ptr 	optional 	optional
#define DT_FINI 	        13 	//d_ptr 	optional 	optional
#define DT_SONAME 	        14 	//d_val 	ignored 	optional
#define DT_RPATH  	        15 	//d_val 	optional 	ignored
#define DT_SYMBOLIC  	    16 	//ignored 	ignored 	optional
#define DT_REL 	            17 	//d_ptr 	mandatory 	optional
#define DT_RELSZ 	        18 	//d_val 	mandatory 	optional
#define DT_RELENT 	        19 	//d_val 	mandatory 	optional
#define DT_PLTREL 	        20 	//d_val 	optional 	optional
#define DT_DEBUG 	        21 	//d_ptr 	optional 	ignored
#define DT_TEXTREL  	    22 	//ignored 	optional 	optional
#define DT_JMPREL 	        23 	//d_ptr 	optional 	optional
#define DT_BIND_NOW      	24 	//ignored 	optional 	optional
#define DT_INIT_ARRAY 	    25 	//d_ptr 	optional 	optional
#define DT_FINI_ARRAY 	    26 	//d_ptr 	optional 	optional
#define DT_INIT_ARRAYSZ 	27 	//d_val 	optional 	optional
#define DT_FINI_ARRAYSZ 	28 	//d_val 	optional 	optional
#define DT_RUNPATH 	        29 	//d_val 	optional 	optional
#define DT_FLAGS 	        30 	//d_val 	optional 	optional
//#define DT_ENCODING 	    32? maybe 31 	//unspecified 	unspecified 	unspecified
#define DT_PREINIT_ARRAY 	32 	//d_ptr 	optional 	ignored
#define DT_PREINIT_ARRAYSZ 	33 	//d_val 	optional 	ignored
#define DT_SYMTAB_SHNDX 	34 	//d_ptr 	optional 	optional
#define DT_LOOS 	        0x6000000D 	//unspecified 	unspecified 	unspecified
#define DT_HIOS 	        0x6ffff000 	//unspecified 	unspecified 	unspecified
#define DT_LOPROC 	        0x70000000 	//unspecified 	unspecified 	unspecified
#define DT_HIPROC 	        0x7fffffff 	//unspecified 	unspecified 	unspecified



unsigned int sht_offset;
int sht_count;
unsigned int ph_offset;
int ph_ent_num;
unsigned int dyn_table_offset;
int dyn_ent_num;
char dynamic_symbol_names[100][100]; //max 100;
int dynamic_symobl_names_index=0;
/*typedef struct {
        unsigned char   e_ident[EI_NIDENT];
        Elf32_Half      e_type;
        Elf32_Half      e_machine;
        Elf32_Word      e_version;
        Elf32_Addr      e_entry;
        Elf32_Off       e_phoff;
        Elf32_Off       e_shoff;
        Elf32_Word      e_flags;
        Elf32_Half      e_ehsize;
        Elf32_Half      e_phentsize;
        Elf32_Half      e_phnum;
        Elf32_Half      e_shentsize;
        Elf32_Half      e_shnum;
        Elf32_Half      e_shtrndx;
} Elf32_Ehdr;

*/


struct sections{
    int index;
    Elf64_Off offset;
    Elf64_Xword size; 
    char name[30];
}total_sections[100];
int total_sections_index=0;
struct sym_tbl_entry{  
    int offset;
    Elf64_Xword size;
    int str_table_index;
    int sec_index;
};
struct relocations_sec{  
    int offset;
    Elf64_Xword size;
    int link_index;
    int sec_index;
};
int str_tables_locs[30]; 
int dyn_tables_locs[30];
struct sym_tbl_entry sym_tables_locs[30];
int hash_tables_locs[30];
struct relocations_sec rel_tables_locs[30];
struct relocations_sec rela_tables_locs[30];
int str_table_index ;
int dyn_table_index ;
int hash_table_index ;
int rel_table_index ;
int rela_table_index ;
int sym_table_index ;
int section_str_tbl_index;
void add_to_str_tables(int offset){
    str_tables_locs[str_table_index++] = offset;
}
void add_to_dyn_tables(int offset){
    dyn_tables_locs[dyn_table_index++] = offset;
}
void add_to_sym_tables(int offset,Elf64_Xword size,int str_tbl,int sec_index){
    
    sym_tables_locs[sym_table_index].offset = offset;
    sym_tables_locs[sym_table_index].size = size;
    sym_tables_locs[sym_table_index].str_table_index = str_tbl;
    sym_tables_locs[sym_table_index].sec_index = sec_index;
    //printf("Added %s\n",name);
    //getchar();
    sym_table_index++;
}
void add_to_hash_tables(int offset){
    hash_tables_locs[hash_table_index++] = offset;
}
void add_to_rel_tables(int offset,Elf64_Xword size,int link_index,int sec_index){
    rel_tables_locs[rel_table_index].offset = offset;
    rel_tables_locs[rel_table_index].size = size;
    rel_tables_locs[rel_table_index].link_index = link_index;
    rel_tables_locs[rel_table_index].sec_index = sec_index;
    rel_table_index++;
}
void add_to_rela_tables(int offset,Elf64_Xword size,int link_index,int sec_index){
    rela_tables_locs[rela_table_index].offset = offset;
    rela_tables_locs[rela_table_index].size = size;
    rela_tables_locs[rela_table_index].link_index = link_index;
    rela_tables_locs[rela_table_index].sec_index = sec_index;
    rela_table_index++;
}

void print_data(char *buf,int size){
    int i;
    
    int last=0;
    for (i=0;i<size;i++){       
        if(buf[i]>=32 && buf[i]<127) 
            printf("%c",buf[i]);
        else 
            printf(".");
    }
    printf("\n");
}
void parse_rel_tables(unsigned char *_file){
    int i =0 ;   
    for (i;i<rel_table_index;i++){
        unsigned int off  = rel_tables_locs[i].offset;
        Elf64_Xword size = rel_tables_locs[i].size;
        int count = size/sizeof(Elf64_Rel_entry);
        
        Elf64_Rel_entry *rel_adr = (Elf64_Rel_entry*) (_file + rel_tables_locs[i].offset);
        printf("\n\nREL Relocation table %s - total entries %d :\n",rel_tables_locs[i].sec_index,total_sections[rel_tables_locs[i].sec_index].name,count);
        printf("------------------------------\n");
        int j=0;
        for (j;j<count;j++,rel_adr++){
            printf("Virtual address\t\t\t0x%08x\n",rel_adr->r_offset);
            int sym_index = ELF64_R_SYM(rel_adr->r_info);
            int rel_type  = ELF64_R_TYPE(rel_adr->r_info);
            printf("Dynamic symbol index\t%d [%s]\n",sym_index,dynamic_symbol_names[sym_index]);
            printf("type\t\t\t\t%d\n",rel_type);
            printf("------------------------------\n");
        }//end j
    }
}
void parse_rela_tables(unsigned char *_file){
    int i =0 ;   
    for (i;i<rela_table_index;i++){
        unsigned int off  = rela_tables_locs[i].offset;
        Elf64_Xword size = rela_tables_locs[i].size;
        int count = size/sizeof(Elf64_Rela_entry);
        
        Elf64_Rela_entry *rela_adr = (Elf64_Rela_entry*) (_file + rela_tables_locs[i].offset);
        printf("\n\nRELA Relocation table %s - total entries %d :\n",total_sections[rela_tables_locs[i].sec_index].name,count);
        printf("------------------------------\n");
        int j=0;
        for (j;j<count;j++,rela_adr++){
            printf("Virtual address\t\t\t0x%08x\n",rela_adr->r_offset);
            int sym_index = ELF64_R_SYM(rela_adr->r_info);
            int rela_type  = ELF64_R_TYPE(rela_adr->r_info);
            printf("Dynamic symbol index\t%d [%s]\n",sym_index,dynamic_symbol_names[sym_index]);
            printf("type\t\t\t\t%d\n",rela_type);
            printf("addend\t\t\t\t0x%0016X\n",rela_adr->r_addend);
            printf("------------------------------\n");
        }//end j
    }
}

void parse_sym_tables(unsigned char *_file){
    int i =0 ; 
    printf("\n\n");
    for (i;i<sym_table_index;i++){
        unsigned int off  = sym_tables_locs[i].offset;
        Elf64_Xword size = sym_tables_locs[i].size;
        int count = size/sizeof(Elf64_Sym_entry);
        
        
        char *adr = _file + off;
        
        char *str_table_sec     = _file + sht_offset + sym_tables_locs[i].str_table_index * sizeof(Elf64_Shdr);
        Elf64_Shdr *shsec       = (Elf64_Shdr*) str_table_sec;
        char *str_table_offset  = _file + shsec->sh_offset;   //associated string table location in array 
        printf("Symbol table %d - name %s - offset 0x%08X - size %ld - total %d entrie[s] (skipping first) - str table %s\n",i,total_sections[sym_tables_locs[i].sec_index].name,off,size,count,total_sections[sym_tables_locs[i].str_table_index].name);
        
        
        unsigned int cur_offset = off;
        char sym_name[100];
        int j=1;
        for (j;j<count;j++){
            
            //The first symbol table entry is reserved and must be all zeroes. start from one
            adr += sizeof(Elf64_Sym_entry);
            cur_offset += sizeof(Elf64_Sym_entry);
            Elf64_Sym_entry *entry = (Elf64_Sym_entry*) adr;
            //printf ("reading from %08x \n",shsec->sh_offset + entry->st_name);
            //printf ("%08x \n",entry->st_name);
            if (*(str_table_offset + entry->st_name) == 0){
                printf("Name[hex]\t\t\t[Empty]\n");
            }else{
                strcpy(sym_name,str_table_offset + entry->st_name);
                printf("Name[hex]\t\t\t%s\n",sym_name);
                if (strcmp(".dynsym",total_sections[sym_tables_locs[i].sec_index].name)==0){
                 
                    strcpy(dynamic_symbol_names[j],sym_name);
                    
                }
            }unsigned char binding_scope = entry->st_info & 0xF0;
            binding_scope >>= 4;
            printf("Symbole scope:\t\t\t");
            switch(binding_scope){
            case 0:
                printf("Local\n");
                break;
            case 1:
                printf("Global\n");
                break;
            case 2:
                printf("Weak global\n");
                break;
            case 10:
                printf("Environment-specific use\n");
                break;
            case 12:
                printf("Undocumented\n");
                break;
            case 13:
                printf("Processor-specific use\n");
                break;
            
            case 15:
                printf("Undocumented\n");
                break;
            default:
                printf("Unknown: 0x%02X\n",binding_scope);
            
            }
            printf("Symbole type:\t\t\t");
            unsigned char sym_type = entry->st_info & 0x0F;
            short section_defined =0;
            switch(sym_type){
            case 0:
                printf("Undefined. Maybe absolute symbol\n");
                break;
            case 1:
                printf("Data object\n");
                break;
            case 2:
                printf("Function entry point\n");
                break;
            case 3:
                printf("Symbol associated with a section\n");
                section_defined = 1;
                break;
            case 4:
                printf("Source file associated with the object type\n");
                break;
            case 10:
                printf("Environment-specific use\n");
                break;
            case 12:
                printf("Symbol associated with a section\n");
                break;
            case 13:
                printf("Processor-specific use\n");
                break;
            case 15:
                printf("Symbol associated with a section\n");
                break;
            default:
                printf("Unknown: 0x%02X\n",sym_type);
            
            }
            if (entry->st_shndx < total_sections_index) printf("Defined in section\t\t\t%d [%s]\n",entry->st_shndx,total_sections[entry->st_shndx].name);
            printf("Value\t\t\t0x%0016X\n",entry->st_value); //Absolute value or virtual address
            printf("Symbole size\t\t\t\t%d\n",entry->st_size);
            printf("-----------------------------------------\n");
        
        
        }//end of for j
        //getchar();
        
    }//end of for i
}
void check_segment(Elf64_Off start, Elf64_Xword size){
    Elf64_Off end = start + size;
    int i;
    for (i=1;i<total_sections_index;i++){
        if (total_sections[i].offset >= start && total_sections[i].offset <= end){
            
            printf("%s ",total_sections[i].name);
        }
    }   
}
void parse_ident(char *ident){
    printf("File magic num:\t\t\t");
    print_data(ident,4);
    
    int elf_class = ident[4];
    if (elf_class==0){
        printf("Class:\t\t\t\tINVALID\n");
    }else if (elf_class==1){
        printf("Class:\t\t32 bit\n");
    }else if (elf_class==2){
        printf("Class:\t\t\t\t64 bit\n");
    }else{
        printf("Class:\t\t\t\tINVALID\n");
    }
    
    
    int elf_encode = ident[5];
    if (elf_encode==0){
        printf("Encode:\t\t\t\tINVALID\n");
    }else if (elf_encode==1){
        printf("Class:\t\t\t\tLSB (Little Indian)\n");
    }else if (elf_encode==2){
        printf("Class:\t\t\t\tMSB (Big Endian)\n");
    }else{
        printf("Class:\t\t\t\tINVALID\n");
    }
    
    int elf_version = ident[6];
    printf("Version:\t\t\t%d\n",elf_version);
    
    
    int elf_osabi = ident[7];
    switch (elf_osabi){
        case 0:
            printf("OS ABI:\t\t\t\tSystem V ABI\n");
            break;
        case 1:
            printf("OS ABI:\t\t\t\tHP-UX operating system\n");
            break;
        case 255:
            printf("OS ABI:\t\t\t\tStandalone (embedded)\n");
            break;
    }
    
    int elf_abivers = ident[8]; //For applications conforming to the System V ABI, third edition, this field should contain 0
    printf("ABI Version:\t\t\t%d\n",elf_abivers);
}
void parse_elf_header(Elf64_Ehdr *ehdr){
    char ident[16];
    memcpy(ident,ehdr,16);
    parse_ident(ident);
    
    
    switch (ehdr->e_type){
        case 0:
            printf("Object file type:\t\tNo file type\n");
            break;
        case 1:
            printf("Object file type:\t\tRelocatable object file\n");
            break;
        case 2:
            printf("Object file type:\t\tExecutable file\n");
            break;       
        case 3:
            printf("Object file type:\t\tShared object file\n");
            break;       
        case 4:
            printf("Object file type:\t\tCore file\n");
            break;
        case 0xFE00:
            printf("Object file type:\t\tEnvironment-specific use\n");
            break;
        case 0xFEFF:
            printf("Object file type:\t\tUnknown\n");
            break;
        case 0xFF00:
            printf("Object file type:\t\tProcessor-specific use\n");
            break;
        case 0xFFFF:
            printf("Object file type:\t\tUnknown\n");
            break;
    }
    
    printf("Target machine:\t\t\t%d\n",ehdr->e_machine);
    printf("Format version:\t\t\t%d\n",ehdr->e_version);   
    printf("Entry point Virtual address:\t0x%0016X\n",ehdr->e_entry);
    
    printf("Program Header table:\t\t%08X  [FILE OFSSET]\n",ehdr->e_phoff);
    ph_offset = ehdr->e_phoff;
    printf("Section header table:\t\t%08X  [FILE OFSSET]\n",ehdr->e_shoff);
    sht_offset = ehdr->e_shoff;
    printf("Processor specific flags:\t0x%08X\n",ehdr->e_flags);
    printf("Header table size:\t\t%d\n",ehdr->e_ehsize);
    printf("PHT entry size:\t\t\t%d\n",ehdr->e_phentsize);
    printf("PHT entries count:\t\t\%d\n",ehdr->e_phnum);
    ph_ent_num = ehdr->e_phnum;
    printf("SHT entry size:\t\t\t%d\n",ehdr->e_shentsize);
    printf("SHT entries count:\t\t\%d\n",ehdr->e_shnum);
    sht_count = ehdr->e_shnum;
    printf("SHT str-tbl index:\t\t\%d [INDEX IN SHT]\n",ehdr->e_shstrndx);
    section_str_tbl_index = ehdr->e_shstrndx;
}
void parse_section_table(unsigned char *_file){
    int i;
    unsigned char * adr ;  //array address
    int next=0;
    unsigned int cur_offset = sht_offset;
    
    int sh_str_table_offset ;
    printf ("Retrieving shstoff\t\t");
   
    int off_temp = sht_offset + section_str_tbl_index * sizeof(Elf64_Shdr);
    adr = ((unsigned char*)_file) + off_temp;
    Elf64_Shdr *shdr;
    shdr = (Elf64_Shdr *) adr;
    sh_str_table_offset = shdr->sh_offset;
    /*for (i=0;i<=sht_count;i++){
        Elf64_Shdr *shdr;
        shdr = (Elf64_Shdr *) adr;
        adr+= sizeof(Elf64_Shdr); //Next Entry
        cur_offset += sizeof(Elf64_Shdr);
        if (shdr->sh_type==3){
            sh_str_table_offset = shdr->sh_offset;
            break;
        }   
    }*/
    printf ("shstoff as: 0x%08X\n",sh_str_table_offset);
    cur_offset = sht_offset;
    adr = ((unsigned char*)_file) + sht_offset ;
    for (i=0;i<sht_count;i++){
        
        printf("\nSection index %d start at offset 0x%08X\n",i,cur_offset);
        Elf64_Shdr *shdr;
        shdr = (Elf64_Shdr *) adr;
        adr+= sizeof(Elf64_Shdr); //Next Entry
        cur_offset += sizeof(Elf64_Shdr);
        char temp[30];
        strncpy(temp,sh_str_table_offset + shdr->sh_name + _file,30);
        
        total_sections[total_sections_index].index=i;
        strncpy(total_sections[total_sections_index].name,temp,100);
        total_sections[total_sections_index].offset = shdr->sh_offset;
        total_sections[total_sections_index].size   = shdr->sh_size;
        total_sections_index++;
        
        
        switch (shdr->sh_type){
            case SHT_NULL:
                printf("[SHT_NULL] NULL Section. Jumping to next entry\n");
                continue;
                break;
            case SHT_PROGBITS:
                printf("Type:\t\t\t\t[SHT_PROGBITS] Program data\n");
                break;
            case SHT_SYMTAB:
                printf("Type:\t\t\t\t[SHT_SYMTAB] Linker symbol table (holds a symbol table)\n");
                add_to_sym_tables( shdr->sh_offset,shdr->sh_size,shdr->sh_link,i );
                break;
            case SHT_STRTAB:
                printf("Type:\t\t\t\t[SHT_STRTAB] String table\n");
                add_to_str_tables( shdr->sh_offset );
                break;
            case SHT_RELA:
                printf("Type:\t\t\t\t[SHT_RELA] Rela relocation entries table\n");
                add_to_rela_tables( shdr->sh_offset,shdr->sh_size,shdr->sh_link,i );
                break;
            case SHT_HASH:
                printf("Type:\t\t\t\t[SHT_HASH] Symbol hash table\n");
                add_to_hash_tables( shdr->sh_offset );
                break;
            case SHT_DYNAMIC: //Not a symbol table
                printf("Type:\t\t\t\t[SHT_DYNAMIC] Dynamic linking table\n");
                dyn_table_offset = shdr->sh_offset;
                dyn_ent_num = shdr->sh_size/shdr->sh_entsize;
                break;
            case SHT_NOTE:
                printf("Type:\t\t\t\t[SHT_NOTE] Note information\n");
                break;
            case SHT_NOBITS:
                printf("Type:\t\t\t\t[SHT_NOBITS] Uninitialized space; does not occupy any space in the file\n");
                break;
            case SHT_REL:
                printf("Type:\t\t\t\t[SHT_REL] Rel relocation entries table\n");
                add_to_rel_tables( shdr->sh_offset,shdr->sh_size,shdr->sh_link,i );
                break;
            case SHT_SHLIB:
                printf("Type:\t\t\t\t[SHT_SHLIB] Reserved\n");
                break;
            case SHT_DYNSYM:
                printf("Type:\t\t\t\t[SHT_DYNSYM] Dynamic loader symbol table (holds a symbol table)\n");
                add_to_sym_tables( shdr->sh_offset,shdr->sh_size,shdr->sh_link,i );
                break;
            case SHT_INIT_ARRAY:
                printf("Type:\t\t\t\t[SHT_INIT_ARRAY] array of pointers to initialization functions\n");
                break;
            case SHT_FINI_ARRAY:
                printf("Type:\t\t\t\t[SHT_FINI_ARRAY] array of pointers to termination functions\n");
                break;
            case SHT_PREINIT_ARRAY:
                printf("Type:\t\t\t\t[SHT_PREINIT_ARRAY] array of pointers to functions that are invoked before all other initialization functions\n");
                break;
            case SHT_GROUP:
                printf("Type:\t\t\t\t[SHT_GROUP]  defines a section group\n");
                break;
            case SHT_SYMTAB_SHNDX:
                printf("Type:\t\t\t\t[SHT_SYMTAB_SHNDX]  associated with a symbol table section\n");
                break;
            case SHT_LOOS: //SHT_LOOS through SHT_HIOS range mean the same
                printf("Type:\t\t\t\t[SHT_LOOS] Environment-specific use\n");
                break;
            case SHT_HIOS:
                printf("Type:\t\t\t\t[SHT_HIOS] Environment-specific use\n");
                break;
            case SHT_LOPROC: //SHT_LOPROC through SHT_HIPROC range mean the same
                printf("Type:\t\t\t\t[SHT_LOPROC] Processor-specific use\n");
                break;
            case SHT_HIPROC:
                printf("Type:\t\t\t\t[SHT_HIPROC] Processor-specific use\n");
                break;
            case SHT_LOUSER:
                printf("Type:\t\t\t\t[SHT_LOUSER] lower bound of the range of indexes reserved for application programs (user defined)\n");
                break;
            case SHT_HIUSER:
                printf("Type:\t\t\t\t[SHT_HIUSER] upper bound of the range of indexes reserved for application programs (user defined)\n");
                break;
            default:
                printf("Unkown type or in a defined range. 0x%08X\n",shdr->sh_type);
        }
        printf("str_name offset \t\t%d\n",shdr->sh_name);
        
        printf("Name\t\t\t\t%s\n",temp);
        printf("Section flags \t\t\t0x%0016X\n",shdr->sh_flags);
        if (shdr->sh_addr==0)
            printf("Virtual address \t\tNo allocation\n");
        else
            printf("Virtual address \t\t0x%0016X\n",shdr->sh_addr);
        printf("Section location \t\t0x%08X [FILE OFFSET]\n",shdr->sh_offset);
        printf("Section size\t\t\t0x%0016X\n",shdr->sh_size);
        printf("Link index\t\t\t%d  ",shdr->sh_link);
        switch (shdr->sh_type){
        case SHT_DYNAMIC:
            printf("[String table used by entries in this section]\n");
            break;
        case SHT_HASH:
            printf("[Symbol table to which the hash table applies]\n");
            break;
        case SHT_REL:
            printf("[Symbol table referenced by relocations]\n");
            break;
        case SHT_RELA:
            printf("[Symbol table referenced by relocations]\n");
            break;
        case SHT_SYMTAB:
            printf("[String table used by entries in this section]\n");
            break;      
        case SHT_DYNSYM:
            printf("[String table used by entries in this section]\n");
            break;
        default:
            //printf("[Undefined role]\n");
            printf("\n");
        }
        printf("sh_index Info\t\t\t%d\n",shdr->sh_info);
        printf("Section alignment\t\t%d\n",shdr->sh_addralign);
        if (shdr->sh_entsize==0)
            printf("In-table entry size\t\tNot containing one\n");
        else
            printf("In-table entry size\t\t%d\n",shdr->sh_entsize);
          
        
        //getchar();
    }
    
}
void parse_ph(unsigned char *_file){
    int i;
    unsigned char * adr = _file + ph_offset;  //array address
    Elf64_Phdr_entry * phe = (Elf64_Phdr_entry*)adr;
    printf("\n\nProgram Header Table:\n\n");
    for (i=0 ; i<ph_ent_num;i++,phe++){
        printf("Type:\t\t\t\t");
        switch(phe->p_type){
            case PT_NULL:
                printf("[PT_NULL] unused entry\n");
                break;
            case PT_LOAD:
                printf("[PT_LOAD] loadable segment\n");
                break;
            case PT_DYNAMIC:
                printf("[PT_DYNAMIC] dynamic linking information\n");
                break;
            case PT_INTERP:
                printf("[PT_INTERP] location and size of a null-terminated path name to invoke as an interpreter\n");
                break;
            case PT_NOTE:
                printf("[PT_NOTE] specifies the location and size of auxiliary information\n");
                break;
            case PT_SHLIB:
                printf("[PT_SHLIB] reserved\n");
                break;
            case PT_PHDR:
                printf("[PT_PHDR] specifies the location and size of the program header table itself\n");
                break;
            case PT_TLS:
                printf("[PT_TLS] specifies the Thread-Local Storage template\n");
                break;
            case PT_LOOS ... PT_HIOS: 
                printf("[PT_LOOS - PT_HIOS] reserved\n");
                break;
            case PT_LOPROC ... PT_HIPROC: 
                printf("[PT_LOPROC - PT_HIPROC] reserved\n");
                break;
            default:
                printf("0x%08X unspecified\n",phe->p_type);  
            
        
        }
        
        printf("Segment start:\t\t\t0x%0016X [File Offset]\n",phe->p_offset);
        printf("Virtual addrss:\t\t\t0x%0016X\n",phe->p_vaddr);
        printf("Pyhical addr (reserved meaning)\t0x%0016X\n",phe->p_paddr);
        printf("File segment size\t\t0x%0016X\n",phe->p_filesz);
        printf("Memory segment size\t\t0x%0016X\n",phe->p_memsz);
        printf("Flags:\t\t\t\t0x%0016X\n",phe->p_flags);
        printf("p_align:\t\t\t0x%0016X\n",phe->p_align);
        printf("This segment contains following sections:\n");
        check_segment(phe->p_offset,phe->p_filesz);
        printf("\n----------------------------\n");
    }
}
void parse_dyn_table(unsigned char * _file){
    int i;
    unsigned char * adr = _file + dyn_table_offset;  //array address
    Elf64_Dyn_entry * dte = (Elf64_Dyn_entry*)adr;
    printf("\n\nProgram Dynamic Table:\n\n");
    for (i=0 ; i<dyn_ent_num;i++,dte++){
        printf("Tag:\t\t\t\t");
        switch (dte->d_tag){
        case DT_NULL:
            printf("NULL [END OF ENTRIES]\n");
            break;
        case DT_NEEDED:
            printf("[DT_NEEDED] a string table offset of a library name");
            break;
        case DT_PLTRELSZ:
            printf("[DT_PLTRELSZ] holds the total size, in bytes, of the relocation entries associated with the PLT");
            break;
        case DT_PLTGOT:
            printf("[DT_PLTGOT] holds an address associated with the PLT or GOT");
            break;
        case DT_HASH:
            printf("[DT_HASH] holds the address of the symbol hash table");
            break;
        case DT_STRTAB: //vaddr of dynstr
            printf("[DT_STRTAB] holds the address of the string table");    
            break;
        case DT_SYMTAB: //vaddr of dynsym
            printf("[DT_SYMTAB] holds the address of the symbol table,");
            break;
        case DT_RELA:
            printf("[DT_RELA] holds the address of a relocation table");
            break;
        case DT_RELASZ:
            printf("[DT_RELASZ] holds the total size of the DT_RELA relocation table");
            break;
        case DT_RELAENT:
            printf("[DT_RELAENT] holds the size of the DT_RELA relocation entry");
            break;
        case DT_STRSZ:
            printf("[DT_STRSZ] holds the size of the string table");
            break;
        case DT_SYMENT:
            printf("[DT_SYMENT] holds the size of a symbol table entry");
            break;
        case DT_INIT:
            printf("[DT_INIT] holds the address of the initialization function");
            break;
        case DT_FINI:
            printf("[DT_FINI] holds the address of the termination function");
            break;
        case DT_SONAME:
            printf("[DT_SONAME] holds the string table offset of the name of the shared object");
            break;
        case DT_RPATH:
            printf("[DT_RPATH] holds the string table offset of search library search path string");
            break;
        case DT_SYMBOLIC:
            printf("[DT_SYMBOLIC] alters the dynamic linker's symbol resolution algorithm for references within the library.");
            break;
        case DT_REL:
            printf("[DT_REL] similar to DT_RELA, except its table has implicit addends");
            break;
        case DT_RELSZ:
            printf("[DT_RELSZ] holds the total size of the DT_REL relocation table");
            break;
        case DT_RELENT:
            printf("[DT_RELENT] holds the size of the DT_REL relocation entry");
            break;
        case DT_PLTREL:
            printf("[DT_PLTREL] specifies the type of relocation entry to which the PLT refers");
            break;        
        case DT_DEBUG:
            printf("[DT_DEBUG] used for debugging");
            break;
        case DT_TEXTREL:
            printf("[DT_TEXTREL] modification to a non-writable segment");
            break;
        case DT_JMPREL:
            printf("[DT_JMPREL] holds the address of relocation entries associated solely with PLT");
            break;
        case DT_BIND_NOW:
            printf("[DT_BIND_NOW] tells to process all relocations beforehand");
            break;
        case DT_INIT_ARRAY:
            printf("[DT_INIT_ARRAY] holds the address of the array of pointers to initialization functions");
            break;
        case DT_FINI_ARRAY:
            printf("[DT_FINI_ARRAY] holds the address of the array of pointers to termination functions");
            break;
        case DT_INIT_ARRAYSZ:
            printf("[DT_INIT_ARRAYSZ] holds the size of the array of initialization functions pointed to by the DT_INIT_ARRAY entry");
            break;
        case DT_FINI_ARRAYSZ:
            printf("[DT_FINI_ARRAYSZ] holds the size of the array of termination functions pointed to by the DT_FINI_ARRAY entry");
            break;
        case DT_RUNPATH:
            printf("[DT_RUNPATH] holds the string table offset of library search path string");
            break;
        case DT_FLAGS:
            printf("[DT_FLAGS] holds flag values specific to the object");
            break;
        case DT_PREINIT_ARRAY:
            printf("[DT_PREINIT_ARRAY] holds the address of the array of pointers to pre-initialization functions");
            break;
        case DT_PREINIT_ARRAYSZ:
            printf("[DT_PREINIT_ARRAYSZ] holds the size of the array of pre-initialization functions pointed to by the DT_PREINIT_ARRAY entry");
            break;          
        case DT_SYMTAB_SHNDX:
            printf("[DT_SYMTAB_SHNDX] holds the address of the SHT_SYMTAB_SHNDX section associated with the dynamic symbol table referenced by the DT_SYMTAB element");
            break;
        case DT_LOOS ... DT_HIPROC:
            printf("check docs...");
            break;
        default:
            printf("[0x%0016X] Unkown\n",dte->d_tag);
        }
        printf("\nd_un\t\t\t\t0x%0016X\n-------------------\n",dte->d_un);
    
    }
}
int main(int argc , char** argv){
    if (argc<2){
        printf("Specify the object file\n");
        return -1;
    }
    Elf64_Ehdr ehdr;
    
    char *path = argv[1];
    char *p = buf;
    FILE *f = fopen(path,"r");
    int c;
    while ((c=getc(f))!=EOF){
        *p++ = (unsigned char)c;       
    }
    printf("\n");
    
    memcpy(&ehdr,buf,sizeof(ehdr));
    parse_elf_header(&ehdr);printf("here");
    parse_section_table(buf);
    parse_sym_tables(buf);
    
    parse_rel_tables(buf);
    
    parse_rela_tables(buf);
    
    
    parse_ph(buf);
    
    
    parse_dyn_table(buf);
    
    
    
    printf("\n");
    return 0;
    
}
