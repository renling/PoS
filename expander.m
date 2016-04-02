function d = expander(a, b)
d(1) = (Hb(a) + Hb(b)) / (Hb(a) - b*Hb(a/b));
d(2) = (Hb(a) + Hb(b)) / a / -log2(b);
end

function h = Hb(p)
h = - p*log2(p) - (1-p)*log2(1-p);
end 