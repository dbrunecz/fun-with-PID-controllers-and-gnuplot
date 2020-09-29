function plot_ctrl(fname)
    dat = load(fname);
    t = dat(:,1);
    v = dat(:,3);
    u = dat(:,6);

    figure();
    p = plotyy(t, v, t, u);
    %ylabel (p1(1), "Watts");
    %xlabel("time (ms)");
end
