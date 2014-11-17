#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/Tooling.h"

#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/CommandLine.h"
#include "clang/Basic/LangOptions.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("my-tool options");


class DumpCallback :public MatchFinder::MatchCallback {

    virtual void run(const MatchFinder::MatchResult &Result){
        
        LangOptions LangOpts;
        LangOpts.CPlusPlus = true;
        PrintingPolicy Policy(LangOpts);
        
        const CXXMemberCallExpr *C = Result.Nodes.getNodeAs<CXXMemberCallExpr>("x");

        CXXMethodDecl* method = C->getMethodDecl();
        CXXRecordDecl* staticTyp = C->getRecordDecl();

        if(method->isVirtual()) {
            llvm::errs() << "-----\n";
            llvm::errs() << "new virtual callsite: ";
            C->printPretty(llvm::errs(), 0, Policy);
            llvm::errs() << "\ntype of the callee: " << staticTyp->getName() << "\n"; 
        }
    }
};

int main(int argc, const char **argv){
    DumpCallback Callback;
    MatchFinder Finder;
    Finder.addMatcher(memberCallExpr().bind("x"), &Callback);
    
    CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
    ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());
    return Tool.run(newFrontendActionFactory(&Finder).get());
}
