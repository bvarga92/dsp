filename = 'output.dat';
play_sound = true;
save_sound = false;
fs = 48000;

fid = fopen(filename, 'rt');
signal = double(fscanf(fid, '%d %d\n', [2 Inf])') / 32768;
fclose(fid);

t = (0:size(signal, 1)-1)/fs;
subplot(211);
plot(t, signal(:, 1));
xlabel('t [s]');
title('Bal csatorna');
subplot(212);
plot(t, signal(:, 2));
xlabel('t [s]');
title('Jobb csatorna');

if play_sound
    soundsc(signal, fs);
end
if save_sound
    audiowrite('output.wav', signal, fs);
end
