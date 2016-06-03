function [d1, d2] = expander(a, b, c)
if nargin < 3
    c = 1;
end 
d1 = (Hb(a) + c*Hb(b)) / (Hb(a) - b*Hb(a/b));
d2 = (Hb(a) + c*Hb(b)) / a / -log2(b);
end

