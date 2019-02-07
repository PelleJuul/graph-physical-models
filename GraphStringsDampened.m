clear all;

%% Global setup
fs = 44100;
N = fs * 4;
waveSpeed = 200;
peakCenter = 0.5;
peakAmp = 1.0;
peakSize = 0.5;
damp = 1.0;

%% Model setup
m = GraphModel();
s1 = m.addString(10);
s2 = m.addString(6);
s3 = m.addString(12);

m.connectStrings(1, 2);
m.connectStrings(1, 3);
m.connectStrings(2, 1);

m.addDirichlet(s1, 1);
m.addDirichlet(s1, 8);
m.addNeumann(s3, 12);

%% Model excitement (raised cosine)
L = length(m.strings{1});

for i = 1:L
    if i >= (peakCenter * L) - (peakSize * L) && i < (peakCenter * L) + (peakSize * L)
        m.strings{1}(i) = (peakAmp / 2) * (1 + cos(pi * (i - peakCenter * L) / (peakSize * L)));
    else
        m.strings{1}(i) = 0;
    end
end

m.strings1 = m.strings;

%% Derived parameters
k = 1 / fs;
h = 1 / m.getNodeCount();
y = zeros(N, 1);
rh2 = 1 / h^2;

fprintf("lambda = %f\n", (waveSpeed * k / h));

%% Render

for n = 1:N 
    if (false && mod(n, 10) == 1)
       plot(1:11, [0 m.strings{1}], '-o');
       hold on;
       plot(12:22, [m.strings{2} 0], '-o');
       hold off;
       ylim([-1 1]);
       grid on;
       pause();
    end
    
    for s = 1:size(m.strings, 2)
        for l = 1:size(m.strings{s}, 2)
            u = m.strings{s}(l);
            u1 = m.strings1{s}(l);
            d = rh2 * m.getDxx(s, l);
            m.stringsTemp{s}(l) = (1 / (1 + damp * k)) * (k^2 * waveSpeed^2 * d + 2*u - u1 + damp * k * u1);
        end
    end
    
    m.strings1 = m.strings;
    m.strings = m.stringsTemp;
    y(n) = m.strings{1}(4);
end
