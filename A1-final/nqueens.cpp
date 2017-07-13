
#include <gecode/int.hh> 
#include <gecode/search.hh>
#include <gecode/minimodel.hh>
#include <gecode/gist.hh>



using namespace Gecode;

class Queens : public Space {
    
public:
  /// Position of queens on boards
  IntVarArray q;
  static const int size=9;

  /// The actual problem
  Queens(const int nqueens) : q(*this,nqueens*nqueens,0,1) {
      
    Matrix<IntVarArray> m(q, nqueens, nqueens);

    // one queen per row & per column  
    
    for (int i = 0; i < nqueens; i++){
        linear(*this, m.col(i), IRT_EQ, 1);
        linear(*this, m.row(i), IRT_EQ, 1);

    }
    // at most one queen per diagonal  
    int r=0;
    int c=1;
    for (int k=0; k<(2*(nqueens)-3); k++) {
			IntVarArgs diagF;
                        int i,j;
                        //Check lower diagonal on left side
			for (i=r, j=c; j >= 0 && i < nqueens; i++, j--) {
				diagF<< m(j,i);
			}
                        if (c<nqueens-1)
                        c=c+1;
                        
                        else if (c==nqueens-1)
                        r=r+1;
			linear(*this, diagF,IRT_LQ, 1);
		}
    
    int rr=1;
    int cc=nqueens-1;
    for (int k=0; k<2*(nqueens)-3; k++) {
			IntVarArgs diagB;
                        int i,j;
                        //Check upper diagonal on left side
			for (i = rr,  j = cc; i >= 0 && j >= 0; i--, j--) {
				diagB<< m(j,i);
			}
                        if (rr==nqueens-1)
                        cc--;
                        else if (rr<nqueens-1)
                        rr++;
			linear(*this, diagB,IRT_LQ, 1);
		} 
//    // First-fail heuristic
//    branch(*this, q, INT_VAR_SIZE_MIN(), INT_VAL_MAX());
    //BRANCH_DEGREE_MAX
//    branch(*this, q, INT_VAR_DEGREE_MAX(), INT_VAL_MAX());
    //BRANCH_DEGREE_MIN:
    branch(*this, q, INT_VAR_DEGREE_MIN(), INT_VAL_MAX());
//    //BRANCH_NONE:
//    branch(*this, q, INT_VAR_NONE(), INT_VAL_MAX());

  }
  
  Queens(bool share, Queens& qu) : Space(share,qu) {
    q.update(*this, share, qu.q);
  }
  
  virtual Space* copy(bool share) {
    
    return new Queens(share,*this);
    
  }
  
  void print(std::ostream& os) const {

    Matrix<IntVarArray> m(q, size, size);

    for (int c = 0; c < size; c++) {
        for (int l = 0; l < size; l++){
            os <<"-+-";
        }
        os << std::endl;
      for (int r = 0; r < size; r++) {

	os << m(c,r) << "| ";

      }

      os << std::endl;

    }

  }
  
};

int main(int argc, char* argv[]) {
  
  
  Queens * queens = new Queens(Queens::size);

  // uncomment to see the result on Gist
//  Gist::Print<Queens> p("Print solution");
//  Gist::Options o;
//  o.inspect.click (&p);
//  Gist::dfs(queens,o);
//  delete queens;
  
  DFS<Queens> dfs(queens);
  delete queens;
  int n=1;
  while (Queens* q = dfs.next()) {
      std::cout <<"solution No: "<< n<<std::endl;
      q->print(std::cout);
      std::cout << std::endl;
      n++;
      std::cout<<"depth: "<<dfs.statistics().depth<<std::endl;
      std::cout<<"node: "<<dfs.statistics().node<<std::endl;
      std::cout<<"propagation: "<<dfs.statistics().propagate<<std::endl;
      std::cout<<"failures: "<<dfs.statistics().fail<<std::endl;
      std::cout<<"///////////////////////"<<std::endl;
      delete q;
                }
  

  return 0;
}

