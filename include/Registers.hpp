#ifndef D992CAF6_EF4E_436C_AE75_5B445B20DA30
#define D992CAF6_EF4E_436C_AE75_5B445B20DA30

#include "defines.hpp"
#include "pipeline_phases.hpp"

void print_wb(std::ostream& out, const std::string& prefix, const WB& wb);

void print_mem(std::ostream& out, const std::string& prefix, const MEM& mem);

void print_ex(std::ostream& out, const std::string& prefix, const EX& ex);

#endif /* D992CAF6_EF4E_436C_AE75_5B445B20DA30 */
