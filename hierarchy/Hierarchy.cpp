#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/Tooling.h"

#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/CommandLine.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

class DumpCallback :public MatchFinder::MatchCallback {

    virtual void run(const MatchFinder::MatchResult &Result){
        const CXXRecordDecl *D = Result.Nodes.getNodeAs<CXXRecordDecl>("x");
        if(D->isCompleteDefinition() && D->isClass()){
            llvm::errs() << "-----\n";
            llvm::errs() << D->getKindName() << " " << D->getName() << "\n";
            
            CXXRecordDecl::base_class_const_iterator iter;
           
            if(D->getNumBases()){
                llvm::errs() << "base classes (" << D->getNumBases() << ") :\n";
                for(iter = D->bases_begin(); iter != D->bases_end(); ++iter){
                    llvm::errs() << "  - " << iter->getType().getAsString() << "\n";
                }
            }

            if(D->getNumVBases()){
                llvm::errs() << "virtual base classes (" << D->getNumVBases() << ") :\n";
                for(iter = D->vbases_begin(); iter != D->vbases_end(); ++iter){
                    llvm::errs() << "  - " << iter->getType().getAsString() << "\n";
                }
            }

            llvm::errs() << "virtual methods : \n";
            CXXRecordDecl::method_iterator iter2;
            for(iter2 = D->method_begin(); iter2 != D->method_end();++iter2){
                if(iter2->isVirtual() && iter2->getIdentifier()){
                    llvm::errs() << "  - " << iter2->getName() << "\n";
                }
            }
            llvm::errs() << "\n";
        }
    }
};

int main(int argc, const char **argv){
    DumpCallback Callback;
    MatchFinder Finder;
    Finder.addMatcher(recordDecl().bind("x"), &Callback);
    
    CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
    ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());
    return Tool.run(newFrontendActionFactory(&Finder).get());
}
