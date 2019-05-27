#include <stdio.h>
#include <stdlib.h>

/* Define page table as dynamic structure containing virtual page and page frame
   and initialize variable as pointer to structure */
struct node {
    int vp;
    int pf;
} *pagetable = NULL;
typedef struct node entry;
/* Declare global var's */
int numentries;
int pagesize;
int rp;//global?

/**************************************************************/
void OPTION1() {
/* Declare local var's */
    int mmsize;
    int i;
/* Prompt for total page frame area size, individual page size, and replacement policy */
    printf("Enter physical memory size(words): \n");
    scanf("%d", &mmsize);

    printf("Enter page size(words/page): \n");
    scanf("%d", &pagesize);

    printf("Enter replacement policy(0=LRU, 1=FIFO): \n");
    scanf("%d", &rp);
/* Allocate and initialize page table based on number of entries */
    numentries = mmsize / pagesize;
    pagetable = (entry *) malloc(numentries * sizeof(entry));
    for (i = 0; i < numentries; i++) {
        pagetable[i].pf = i;
        pagetable[i].vp = -1;
    }
    return;
}


/***************************************************************/
void OPTION2() {
/* Declare local var's */
    int vmaddr;
    int i = 0;
    int j;
    int k;
    int offset;
    int realaddr;
    int vp2;//is it 2?
    int pf;
/* Prompt for virtual address */
    printf("Enter virtual memory address to access: \n");
    scanf("%d", &vmaddr);
/* Translate virtual address to virtual page and offset*/
    vp2 = (vmaddr / pagesize);//pagesize? says pmsize physical
    offset = vmaddr % pagesize;
/* Check for end of table, unallocated entry, or matched entry in table
 while none of three cases, keep looping */
    while ((i < numentries) && (pagetable[i].vp != -1) && (pagetable[i].vp != vp2)) {
        i++;
    }
/* In case of end of table, replace either LRU or FIFO entry (top entry in page table), print message */
    if (i == numentries) {
        pf = pagetable[0].pf;
        for (j = 0; j < numentries - 1; j++) {
            pagetable[j] = pagetable[j + 1];
        }
        pagetable[numentries - 1].vp = vp2;
        pagetable[numentries - 1].pf = pf;
        printf("Page fault \n");
    }
/* In case of unallocated entry, set entry according to virtual page and page frame, print message */
    else if (pagetable[i].vp == -1) {
        pagetable[i].vp = vp2;
        pagetable[i].pf = i;
        printf("Page fault \n");
    }
/* In case of hit in page table, calculate physical address and print message, update page table if LRU policy */
    else {
        realaddr = pagetable[i].pf * pagesize + offset;
        printf("Virtual address %d maps tp physical address %d \n", vmaddr, realaddr);
        if (rp == 0) {//we find hit but table isnt full, find last entry in tableand where hit is move hit to last entryand everything below last entry is moved up. check what should do if full table
            while ((k < numentries) && (pagetable[k].vp != -1)) {
                k++;
            }
            pf = pagetable[i].pf;
            vp2 = pagetable[i].vp;
            for (j = i; j < k - 1; j++) {
                pagetable[j] = pagetable[j + 1];
            }
            pagetable[k - 1].vp = vp2;
            pagetable[k - 1].pf = pf;
        }
    }
    return;
}


/***************************************************************/
void OPTION3() {
/* Declare local var's */
    int i;
/* For each valid entry in page table */
    /* print virtual page number and corresponding page frame number */
    printf("VP  |PF  \n");
    for (i = 0; i < numentries; i++) {
        if (pagetable[i].vp != -1) {
            printf("----------\n");
            printf("%d  |%d  \n", pagetable[i].vp, pagetable[i].pf);
        }
    }
    return;
}


/**************************************************************/
int main() {
/* Declare local var's */
    int input;
/* Until user quits */
    /* print menu of options, prompt for user input, and select appropriate option */
    while (input < 4) {
        printf("Virtual memory to Physical memory mapping: \n");
        printf("----------------------------------------- \n");
        printf("1) Enter parameters \n");
        printf("2) Map virtual address to physical address \n");
        printf("3) Print page table \n");
        printf("4) Quit \n");
        scanf("%d", &input);

        switch (input) {
            case 1:
                OPTION1();
                break;

            case 2:
                OPTION2();
                break;

            case 3:
                OPTION3();
                break;

            case 4:
                break;

            default:
                printf("Invalid choice. \n");
                main();


        }
    }
}

