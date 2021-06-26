void enable_paging() {
    // TODO: Homework 2: Enable paging
    // After initializing the page table, write to register SATP register for kernel registers.
    // Flush the TLB to invalidate the existing TLB Entries
    // Suggested: 2 LoCs
	w_satp(MAKE_SATP(kernel_pagetable));
	flush_tlb();
}

// Return the address of the PTE in page table *pagetable*
// The Risc-v Sv48 scheme has four levels of page table.
// For VA:
//   47...63 zero
//   39...47 -- 9  bits of level-3 index
//   30...38 -- 9  bits of level-2 index
//   21...29 -- 9  bits of level-1 index
//   12...20 -- 9  bits of level-0 index
//   0...11  -- 12 bits of byte offset within the page
// Return the last-level page table entry.
static pte_t* pt_query(pagetable_t pagetable, vaddr_t va, int alloc){
    if(va >= MAXVA) BUG_FMT("get va[0x%lx] >= MAXVA[0x%lx]", va, MAXVA);
    // Suggested: 18 LoCs
    for(int level=3;level>0;--level){
    	pte_t *pte=&pagetable[PX(level,va)];
    	if(*pte & PTE_V){
    		pagetable=(pagetable_t)PTE2PA(*pte);
    	}else{
    		if(!alloc||(pagetable=(pde_t*)mm_kalloc())==0)
    			return 0;
    		memset(pagetable,0,PGSIZE);
    		*pte=PA2PTE(pagetable)| PTE_V;
    	}
    }
    return &pagetable[PX(0,va)];
}
int pt_map_pages(pagetable_t pagetable, vaddr_t va, paddr_t pa, uint64 size, int perm){
    // Suggested: 11 LoCs
    uint64 a=PGROUNDDOWN(va),last=PGROUNDDOWN(va+size-1);
    pte_t *pte;
    while(1){
    	if((pte=pt_query(pagetable,a,1))==0 || *pte & PTE_V)
    		return -1;
    	*pte=PA2PTE(pa) | perm | PTE_V;
		//printk("pt_map_pages:%lu %lu\n",a,last);
    	if(a==last) break;
    	a+=PGSIZE;
    	pa+=PGSIZE;
    }
    return 0; // Do not modify
}

paddr_t pt_query_address(pagetable_t pagetable, vaddr_t va){
    // Suggested: 3 LoCs
    pte_t *pte=pt_query(pagetable, va, 0);
    if(pte==0 || (*pte & PTE_V)==0) return 0;
    return PTE2PA(*pte) | (va&0xfff);
}

int pt_unmap_addrs(pagetable_t pagetable, vaddr_t va){
    // Suggested: 2 LoCs
	pte_t *pte=pt_query(pagetable,va,1);
	if(pte==0 || (*pte & PTE_V)==0 ) return -1;
	*pte=0;
	return 0; // Do not modify
}

int pt_map_addrs(pagetable_t pagetable, vaddr_t va, paddr_t pa, int perm){
    // Suggested: 2 LoCs
    pte_t *pte=pt_query(pagetable,va,1);
	if(pte==0 || (*pte & PTE_V)==0) return -1;
	*pte=PA2PTE(pa) | perm | PTE_V;
    return 0; // Do not modify
}
