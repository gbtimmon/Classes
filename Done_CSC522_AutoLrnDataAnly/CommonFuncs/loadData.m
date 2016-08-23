function [ obj,train, groundT, test, testGT] = loadData( )
    obj = load('mnist_all.mat');

    train =[ obj.train0;obj.train1;obj.train2;obj.train3;obj.train4;
             obj.train5;obj.train6;obj.train7;obj.train8;obj.train9;];
         
    test =[ obj.test0;obj.test1;obj.test2;obj.test3;obj.test4;
            obj.test5;obj.test6;obj.test7;obj.test8;obj.test9;];

     groundT = [    ones(size(obj.train0,1),1)*0;
                ones(size(obj.train1,1),1)*1;
                ones(size(obj.train2,1),1)*2;
                ones(size(obj.train3,1),1)*3;
                ones(size(obj.train4,1),1)*4;
                ones(size(obj.train5,1),1)*5;
                ones(size(obj.train6,1),1)*6;
                ones(size(obj.train7,1),1)*7;
                ones(size(obj.train8,1),1)*8;
                ones(size(obj.train9,1),1)*9; ];
            
            
 testGT =  [    ones(size(obj.test0,1),1)*0;
                ones(size(obj.test1,1),1)*1;
                ones(size(obj.test2,1),1)*2;
                ones(size(obj.test3,1),1)*3;
                ones(size(obj.test4,1),1)*4;
                ones(size(obj.test5,1),1)*5;
                ones(size(obj.test6,1),1)*6;
                ones(size(obj.test7,1),1)*7;
                ones(size(obj.test8,1),1)*8;
                ones(size(obj.test9,1),1)*9; ];

train = double(train);
test = double(test);
end

