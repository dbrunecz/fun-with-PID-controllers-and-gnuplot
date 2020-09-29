function plot_xv(fname)
    dat = load(fname);
    t = dat(:,1);
    x = dat(:,2);
    v = dat(:,3);

    figure();
    p = plotyy(t, x, t, v);
    %ylabel (p1(1), "Watts");
    %xlabel("time (ms)");
end
