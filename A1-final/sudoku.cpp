#include <gecode/int.hh> 
#include <gecode/search.hh>
#include <gecode/minimodel.hh>
#include "examples.h"

using namespace Gecode;

class Sudoku : public Space { 
protected: 
	IntVarArray l;
        // the size of matrix
        static const int rows=9;
        static const int columns=9;

public: 
	// Constructor: takes the initial input
	Sudoku(int sudoku[columns][rows], IntConLevel propS) : l(*this, rows*columns, 1, 9) {

		Matrix<IntVarArray> matrix(l, columns, rows); 
                //takes the initial input
		for(int column=0; column<9; column++)
			for(int row=0; row<9; row++)
			if(sudoku[column][row] != 0) 
                            dom(*this, matrix(column,row), sudoku[column][row], sudoku[column][row]);


                        
                // Constraints for rows
		for (int row=0; row<9; row++) 
			distinct(*this, matrix.row(row), propS);
                // Constraints for columns
		for (int column=0; column<9; column++) 
			distinct(*this, matrix.col(column), propS); 
                // Constraints for 3*3 squares
		for (int smallcolumn=0; smallcolumn<9; smallcolumn+=3) 
			for (int smallrow=0; smallrow<9; smallrow+=3) 
			distinct(*this, matrix.slice(smallcolumn, smallcolumn+3, smallrow, smallrow+3), propS);
                
                

                   // Use minimum size
                    branch(*this, l, INT_VAR_SIZE_MIN(), INT_VAL_MIN());

//    //BRANCH_DEGREE_MAX
//    branch(*this, l, INT_VAR_DEGREE_MAX(), INT_VAL_MIN());
//    //BRANCH_DEGREE_MIN:
//    branch(*this, l, INT_VAR_DEGREE_MIN(), INT_VAL_MIN());
////    //BRANCH_NONE:
//    branch(*this, l, INT_VAR_NONE(), INT_VAL_MIN());

	} 

	// Constructor for cloning \a sud
	Sudoku(bool share, Sudoku& sud) : Space(share, sud) { 
		l.update(*this, share, sud.l);
	} 

	// Perform copying during cloning 
	virtual Space* copy(bool share) {
		return new Sudoku(share, *this);
	} 

	// Print solved sudoku 
	void printSolution(std::ostream& x) const { 
		Matrix<IntVarArray> matrix(l, 9, 9);
		for(int i=0; i<9; i++) {
			for(int j=0; j<9; j++) {
				x << matrix(i,j) << " ";
			}
			x << std::endl;
		}
		x << "*********************\n" << std::endl;
	} 
};


int main(int argc, char* argv[]) {
    

                Gecode::IntConLevel propS;
                for(int i=0; i<sizeof(examples)/sizeof(*examples); i++) {
                    bool printed=false;
	 	std::cout << "Sudoku number "<< i << "\n"<<std::endl;
                
                    //selecting propagation options
                   for(int intconlevel=0; intconlevel<4; intconlevel++){
                   if (intconlevel==0) {
                   propS = ICL_DEF;                   
                   } else if (intconlevel==1){
                       propS = ICL_VAL;
                       
                   } else if (intconlevel==2){
                       propS = ICL_BND;
                       
                   } else if (intconlevel==3){
                       propS = ICL_DOM;
                       
                   }
        
                Sudoku* puzzle = new Sudoku(examples[i], propS);
                DFS<Sudoku> dfs(puzzle); 
                delete puzzle;
                
                while (Sudoku* sdk = dfs.next()) {
                    if (!printed){
                        sdk->printSolution(std::cout);
                        printed=true;
                    }
                    if (propS == ICL_DEF) {
                   std::cout << "Default consistency  "<< std::endl;                   
                   } else if (propS == ICL_VAL){
                       std::cout << "Value consistency  "<< std::endl;
                       
                   } else if (propS == ICL_BND){
                       std::cout << "Bound consistency  "<< std::endl;
                       
                   } else if (propS == ICL_DOM){
                       std::cout << "Domain consistency  "<< std::endl;
                       
                   }
                    
                std::cout<<"depth: "<<dfs.statistics().depth<<std::endl;
		std::cout<<"node: "<<dfs.statistics().node<<std::endl;
                std::cout<<"failures: "<<dfs.statistics().fail<<std::endl;
                std::cout<<"\n"<<std::endl;
                
        
		delete sdk;
	}        
                   } 

  


	 	std::cout << std::endl << std::endl;
                std::cout << "//////////////////////"<< std::endl;
	 }
                
	return 0;
}