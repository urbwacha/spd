from ortools.constraint_solver import pywrapcp
from pathlib import Path


class RPQ( ) :
    def __init__ (self,r,p,q):
        self.R = r
        self.P = p
        self.Q = q


def CpSolv( jobs , instanceName ):

    variablesMaxValue = 0
    for i in range (len(jobs)):
        variablesMaxValue += (jobs[ i ].R+jobs[ i ].P+jobs[ i ].Q)
    solver = pywrapcp.Solver('simple_CP', pywrapcp.Solver.DefaultSolverParameters() )
    #variables:
    alfasMatrix = {} # a t ten t ion ! d ic t iona ry − not l i s t !
    for i in range(len(jobs)):
        for j in range(len(jobs)):
            alfasMatrix[i,j]=solver.IntVar(0,1,"alfa "+str( i )+ "_"+str( j ) )
    """
czas zako zanczenia wykonywania poszzeglolnych zadan, równy sumie czasu rozpocz˛ecia oraz
czasu wykonywania,
    """
    starts=[]

    for i in range(len(jobs)):
        starts.append(solver.IntVar(0,variablesMaxValue,"starts"+str(i)))
    cmax = solver.IntVar ( 0 , variablesMaxValue,"cmax")

# c on s t r a in t s :
    for i in range ( len ( jobs ) ) :
        solver.Add( starts[ i ]>=jobs[ i ] . R)
        solver.Add(cmax>= starts[ i ] + jobs [ i ].P+jobs [ i ] .Q)
    for i in range ( len ( jobs ) ) :
        for j in range ( i +1 ,len ( jobs ) ) :
            solver.Add( starts[ i ]+ jobs [ i ].P <= starts[ j ] + alfasMatrix [ i , j ] * variablesMaxValue )
            solver.Add( starts [ j ]+ jobs [ j ].P <= starts [ i ] + alfasMatrix [ j , i ] * variablesMaxValue )
            solver.Add( alfasMatrix [ i , j ] + alfasMatrix [ j , i ] == 1 )

# s o l v e r :
    objective = solver.Minimize(cmax, 1)
    decision_builder = solver.Phase([cmax], solver.CHOOSE_FIRST_UNBOUND, solver.ASSIGN_MIN_VALUE)


    collector = solver.LastSolutionCollector()
    # Add the decision variables.
    for i in starts:
        collector.Add(i)

    collector.AddObjective(cmax)
    solver.Solve(decision_builder, [objective, collector])
    if collector.SolutionCount() > 0:
        best_solution = collector.SolutionCount() - 1

        print(instanceName , "Cmax: ",  collector.ObjectiveValue(best_solution))

        pi = []
        for i in range ( len ( starts) ):
            pi.append ( ( i , collector.Value(best_solution, starts[i]) ) )
        pi.sort ( key=lambda x : x [ 1 ] )
        print(pi)


 ########## ########## ########## ########## ########## ########## ########## ########## ########## ##########
def GetRPQsFromFile ( pathToFile ):
    fullTextFromFile = Path (pathToFile).read_text ( )
    words = fullTextFromFile.replace ( "\n" , " " ).split ( " " )
    words_cleaned = list ( filter (None, words ) )
    numbers = list (map( int , words_cleaned ) )
    numberOfJobs = numbers [ 0 ]
    numbers.pop ( 0 )
    numbers.pop ( 0 )
    jobs = [ ]

    for i in range ( numberOfJobs ) :
        jobs.append (RPQ(numbers [ 0 ] , numbers [ 1 ] , numbers [ 2 ] ) )
        numbers.pop ( 0 )
        numbers.pop ( 0 )
        numbers.pop ( 0 )
    return jobs


if __name__ == '__main__' :
    file_paths = [ "data.txt" ]
    for i in range ( len ( file_paths ) ) :
        jobs = GetRPQsFromFile(file_paths[ i ] )
        CpSolv( jobs , file_paths[ i ] )
