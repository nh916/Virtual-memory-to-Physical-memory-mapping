#include <stdio.h>
#include <stdlib.h>

/* Define page table as dynamic structure containing virtual page and page frame
   and initialize variable as pointer to structure */
struct node {
    int vp;
    int pf;
} *page_table = NULL;
typedef struct node entry;
/* Declare global var's */
int num_entries;
int fifo_or_lru;
int ps;

/**************************************************************/
void option1() {
/* Declare local var's */
    int physical_memory_size;
/* Prompt for total page frame area size, individual page size, and replacement policy */
    printf("Enter physical memory size (words): ");
    scanf("%d", &physical_memory_size);

    printf("Enter page size (words/page): ");
    scanf("%d", &ps);

    printf("Enter replacement policy (0=LRU, 1=FIFO): ");
    scanf("%d", &fifo_or_lru);


/* Allocate and initialize page table based on number of entries */
    num_entries = physical_memory_size / ps;
    page_table = (entry *) malloc(num_entries * sizeof(entry));
//initialize each page_table[i].vp=-1
    for (int i = 0; i < num_entries; i++) {
//        todo check if this is right or not
        page_table[i].vp = -1;
        page_table[i].pf = i;
    }
    return;
}


/***************************************************************/
void option2() {
/* Declare local var's */
    int virtual_memory_address;
    int table_index = 0;
    int actual_address;
    int offset;
    int virtuale_page_2;
    int pf;
    int k = 0;
    int j;
/* Prompt for virtual address. vmaddr */
    printf("Enter virtual memory address to access: ");
    scanf("%d", &virtual_memory_address);
/* Translate virtual address to virtual page and offset*/
/*todo be careful this is also vp here*/
    virtuale_page_2 = virtual_memory_address / ps;
//    offset = virtual_memory_address - (page_table->vp) * (ps);
    offset = virtual_memory_address % ps;

/* Check for end of table, unallocated entry, or matched entry in table
 while none of three cases, keep looping */
//needs to be done in this order or the program will crash
//loop finished
    while ((table_index < num_entries) && (page_table[table_index].vp != -1) &&
           (page_table[table_index].vp != virtuale_page_2)) {
        table_index++;
    }
/* In case of end of table, replace either LRU or FIFO entry (top entry in page table), print message */
    if (table_index == num_entries) {
        pf = page_table[0].pf;
        for (j = 0; j < num_entries - 1; ++j) {
            page_table[j] = page_table[j + 1];
        }
        page_table[num_entries - 1].vp = virtuale_page_2;
        page_table[num_entries - 1].pf = pf;
        printf("Page fault!\n");
    } else if (page_table[table_index].vp == -1) {
        page_table[table_index].vp = virtuale_page_2;
        page_table[table_index].pf = table_index;
        printf("Page fault!\n");
    } else {
        actual_address = page_table[table_index].pf * ps + offset;
        printf("Virtual address %d maps to physical address %d \n", virtual_memory_address, actual_address);
        if (fifo_or_lru == 0) {
            while ((k < num_entries) && (page_table[k].vp != -1)) {
                k++;
            }
            pf = page_table[table_index].pf;
            virtuale_page_2 = page_table[table_index].vp;
            for (j = table_index; j < k - 1; ++j) {
                page_table[j] = page_table[j + 1];
            }
            page_table[k - 1].vp = virtuale_page_2;
            page_table[k - 1].pf = pf;
        }
    }
}


/***************************************************************/
void option3() {
/* Declare local var's */

/* For each valid entry in page table */
    printf("-----------------\n");
    printf("| VP\t | PF\t |\n");
    printf("-----------------\n");
    for (int i = 0; i < num_entries; ++i) {
        if (page_table[i].vp != -1) {
            printf("| %d\t | %d\t |\n", page_table[i].vp, page_table[i].pf);
        }
    }
    printf("-----------------\n");
/* print virtual page number and corresponding page frame number */
//    return;
}


/**************************************************************/
int main() {
/* Declare local var's */
    int choice = 0;
/* Until user quits */
    /* print menu of options, prompt for user input, and select appropriate option */

    while (choice != 4) {
        printf("\nVirtual memory to Physical memory mapping:\n"
               "---------------------------------------\n");
        printf("1) Enter parameters \n");
        printf("2) Map virtual address to physical address\n");
        printf("3) Print page table\n");
        printf("4) Quit\n");

        printf("Enter selection: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                option1();
                break;
            case 2:
                option2();
                break;
            case 3:
                option3();
                break;
            case 4:
                return 000;
//                break;
            default:
                printf("Not a valid choice. Please try again. \n");
                main();
        }
    }




//    return 1;
}

    