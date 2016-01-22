disp('PART A'); 
F   = 'locations.csv';
ll  = xlsread(F, 'B2:C1719');
lat = ll(:,1);
lng = ll(:,2); 

figure
plot(x,y)
