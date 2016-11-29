%% Catmull-rom init
s = 0.5; %tension param
basis = [-s,    2-s,    s-2,    s;
         2*s,   s-3,    3-2*s,  -s;
         -s,    0,      s,      0;
         0,     1,      0,      0];
%% Generate points and pseudo points
 points = [0:1:7; 100, randi(100, 1, 6), 100]';
 points(1,2) = points(2,2);
 points(end, 2) = points(end-1,2);
%% Compute curve
j = 1;
C = @(i,u) [u^3, u^2, u, 1]*basis*points(i-1:i+2, :)
P = zeros(505,2);
for i=2:1:length(points) - 2
    for u = 0:0.01:1
        P(j, :) = C(i,u);
        j = j+1;
    end
end
%% Div 2
p = points
l = C(2,0)
min = l(2)
minPos = l(1)
j = 1;
D2 = @(i,u) [3*u^2, 2*u, 1, 0]*basis*points(i-1:i+2, 2);
for i = 2:1:length(points) - 2
    if((D2(i, 0) <= 0 && D2(i, 1) >= 0) || (D2(i, 0) >= 0 && D2(i, 1) <= 0))
        a = -1.5*p(i-1, 2) + 4.5*p(i, 2) -4.5*p(i+1, 2) + 1.5*p(i+2, 2);
        b = 2*p(i-1, 2) - 5*p(i,2) + 4*p(i+1,2) - 1.*p(i+2,2);
        c = -0.5*p(i-1,2) + 0.5*p(i+1,2);

        sqrtD = sqrt(b^2 - 4*a*c);
        r1 = (-b + sqrtD)/2/a;
        r2 = (-b - sqrtD)/2/a;
        if(r1 >=0  && r1 <= 1)
          l = C(i, r1);
          if(l(2) < min)
              min = l(2);
              minPos = l(1);
          end
        end
        if(r2 >=0  && r2 <= 1)
          l = C(i, r2);
          if(l(2) < min)
              min = l(2);
              minPos = l(1);
          end
        end
    end
end
l = C(length(points) - 2, 1);
if(l(2) < min)
  min = l(2);
  minPos = l(1);
end

%% Plot
figure
plot(points(:,1), points(:,2), 'o', P(:,1), P(:,2), 'b', minPos, min, 'ro')