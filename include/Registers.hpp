#pragma once

#include "defines.hpp"
#include "pipeline_phases.hpp"

void print_wb(std::ostream& out, const std::string& prefix, const WB& wb);

void print_mem(std::ostream& out, const std::string& prefix, const MEM& mem);

void print_ex(std::ostream& out, const std::string& prefix, const EX& ex);
