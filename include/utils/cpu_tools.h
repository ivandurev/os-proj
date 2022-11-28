#ifndef _U_CPU_TOOLS
#define _U_CPU_TOOLS

extern uint64_t get_exception_level();
extern uint64_t get_stack_pointer_level();
extern uint64_t get_pc();

extern void set_el_sp(uint8_t t);

#endif // _U_CPU_TOOLS