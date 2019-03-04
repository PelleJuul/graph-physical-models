clear all;

%% Global setup
fs = 2 * 44100;
N = fs * 3;
waveSpeed = 100;
peakCenter = 0.5;
peakAmp = 1.0;
peakSize = 0.5;

%% Model setup
m = GraphModel();
s1 = m.addString(10);
s2 = m.addString(6);
%s3 = m.addString(3);
%s4 = m.addString(4);

m.connectStrings(1, 2);
%m.connectStrings(1, 3);
%m.connectStrings(2, 1);
%m.connectStrings(s2, s4);
%m.connectStrings(s4, s3);

m.addNeumann(1, 1);
m.addNeumann(2, 6);

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

fprintf("Stability condition: %f < %f (%i)\n", 2 / h, sqrt((2*fs) / waveSpeed), 2 / h < sqrt((2*fs) / waveSpeed));

%% Render

oldMax = max(m.strings{1});

for n = 1:N 
    if (false && mod(n, 100) == 1)
       plot(1:11, [0 m.strings{1}], '-o');
       hold on;
       plot(12:(12+6), [m.strings{2} 0], '-o');
       hold off;
       ylim([-1 1]);
       grid on;
       pause();
    end
    
    for s = 1:size(m.strings, 2)
        for l = 1:size(m.strings{s}, 2)
            u = m.strings{s}(l);
            u1 = m.strings1{s}(l);
            d = rh2 * m.getDxxxx(s, l);
            m.stringsTemp{s}(l) = -k^2 * waveSpeed^2 * d + 2*u - u1;
        end
    end
    
    m.strings1 = m.strings;
    m.strings = m.stringsTemp;
    y(n) = m.strings{1}(4);
    
    if y(n) > 5 * oldMax
        error("Instability detected");
    end
end