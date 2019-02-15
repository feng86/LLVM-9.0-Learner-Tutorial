#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include "HI_print.h"
#include "HI_SimpleTimingEvaluation.h"
#include "polly/PolyhedralInfo.h"

#include <stdio.h>
#include <string>
#include <ios>
#include <stdlib.h>

using namespace llvm;


bool HI_SimpleTimingEvaluation::runOnFunction(Function &F) // The runOnFunction declaration will overide the virtual one in ModulePass, which will be executed for each Function.
{

    SE = &getAnalysis<ScalarEvolutionWrapperPass>().getSE();
    LI = &getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
    LAA = &getAnalysis<LoopAccessLegacyAnalysis>();
    if (F.getName() == top_function_name)
    {
        *Evaluating_log << "Top Function: "<< F.getName() <<" is found and start to evaluate its latency.\n\n\n";
        topFunctionFound = 1;
        top_function_latency = getFunctionLatency(&F);
        *Evaluating_log << "Done latency evaluation of top function: "<< F.getName() <<" and its latency is "<< top_function_latency << "\n\n\n";
    }
    return false;
}

char HI_SimpleTimingEvaluation::ID = 0;  // the ID for pass should be initialized but the value does not matter, since LLVM uses the address of this variable as label instead of its value.

// introduce the dependence of Pass
void HI_SimpleTimingEvaluation::getAnalysisUsage(AnalysisUsage &AU) const {
    AU.setPreservesAll();
    AU.addRequired<LoopInfoWrapperPass>();
    AU.addRequired<ScalarEvolutionWrapperPass>();
    
    // AU.addRequired<ScalarEvolutionWrapperPass>();
    // AU.addRequired<LoopInfoWrapperPass>();
    // AU.addPreserved<LoopInfoWrapperPass>();
    AU.addRequired<LoopAccessLegacyAnalysis>();
    AU.addRequired<DominatorTreeWrapperPass>();
    // AU.addPreserved<DominatorTreeWrapperPass>();
    AU.addRequired<OptimizationRemarkEmitterWrapperPass>();
    AU.addRequiredTransitive<polly::DependenceInfoWrapperPass>();
    AU.addRequired<LoopInfoWrapperPass>();
    AU.addRequiredTransitive<polly::ScopInfoWrapperPass>();
    AU.addRequired<polly::PolyhedralInfo>();
    // AU.addPreserved<GlobalsAAWrapperPass>();
    
}



