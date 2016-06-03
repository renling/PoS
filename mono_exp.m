N = 100;
M = 79;
d = zeros(N,2);
dmin = zeros(N,2);
X = [0.01:0.01:1.0];
for l = 1:N
    c = l / N;
    b = linspace(c, 1, N);
    a = (b - c) / 2;
    
    for i = 1:N
        [d(i,1), d(i,2)] = expander(a(i), b(i));
    end
    dmin(l,:) = min(d);
end


h = figure(1);
LineWidth = 1;
subplot(1, 2, 1);
hold on, grid on
plot(X(1:M), dmin(1:M,1),'LineWidth',LineWidth);
plot(X(1:M), dmin(1:M,2), 'r','LineWidth',LineWidth);
set(gca,'ytick',[0:10:100])
xlabel('\gamma = \beta - 2\alpha')
ylabel('in-degree d')
legend('Chung','Pinsker','Location','northwest')
subplot(1, 2, 2);
hold on, grid on
semilogy(X(M+1:N), dmin(M+1:N,1),'LineWidth',LineWidth);
semilogy(X(M+1:N), dmin(M+1:N,2), 'r','LineWidth',LineWidth);
set(gca,'yscale','log');
xlabel('\gamma = \beta - 2\alpha')
ylabel('in-degree d')
legend('Chung','Pinsker','Location','northwest')


print(h,'gamma_d.pdf','-dpdf','-r0')