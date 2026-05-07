/**
 * @file          ResultAnalysis.hpp
 * @author        zsh945
 * @date          2026-04-26
 */

#ifndef _RESULTANALYSIS_
#define _RESULTANALYSIS_

#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include "Array.hpp"
#include "MacroDef.hpp"
#include "IVPInfo.hpp"
#include "TimeIntegratorFactory.hpp"

void ResultAnalysis(const IVPInfo& ivpInfo, const int CaseLabel);

#endif