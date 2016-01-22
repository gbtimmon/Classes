%
% QUESTION 4
%%%%%%%%%%%%%%%
disp('PART A');
MEAN = 5 ;
VAR  = 1 ;
A = normrnd( MEAN, power(VAR, .5), 4, 4)

disp('PART B');
A2 = A([2,4],:)

disp('PART C');
S1 = sum(A(3,:))
S2 = sum(A(:,4))
S3 = sum(diag(A))

disp('PART D'); 
S4 = sum(sum(A))

%(with a loop as specified with by prompt.)
S5 = 0.0; 
for x = 1:size(A,1)*size(A,2)
    S5 = S5 + A(x)
end
S5

disp('PART E'); 
B = diag([2,3,4,5])

disp('PART F'); 
C = B * A 

disp('PART G'); 
D = A * B

disp('PART H');
X = [1,2,3,4]';
Y = [9,6,4,1]';
COV = cov(X, Y)

%Manually computed

X_diff = ( X - mean(X) );
Y_diff = ( Y - mean(Y) );

mult = 1 / ( size(X, 1) - 1);

COV2 = mult *  [ sum(X_diff .* X_diff), sum(X_diff .* Y_diff);
                 sum(Y_diff .* X_diff), sum(Y_diff .* Y_diff)]
             
disp('PART I'); 
mean(X) ^ 2
