clear all;
clc;

compose_signal = @(left, right) int16([left(:) right(:)]*32768);


filename = 'input.dat';
fs = 48000;
select = 4;

switch select
    case 1  % 440 Hz-es szinusz- es koszinuszjel
        t = 0:1/fs:1;
        signal = compose_signal(sin(2*pi*440*t), cos(2*pi*440*t));

    case 2  % 100 Hz-es haromszogjel a bal csatornan
        t = 0:1/fs:1;
        signal = compose_signal(sawtooth(2 * pi * 100 * t, 0.5), 0 * t);

    case 3  % sweep (chirp) jel a jobb csatornan, pl. szuro meresehez
        t = 0:1/fs:1;
        signal = compose_signal(0 * t, chirp(t, 5000, 1, 15000, 'linear'));

    case 4  % tuskekkel teli szinuszjel a jobb csatornan, pl. medianszuro tesztelesehez
        t = 0:1/fs:0.5;
        x = 0.5 * sin(2 * pi * 100 * t);
        pos = randi(length(x), 1, round(length(x) * 0.01));
        width = randi(6, size(pos));
        height = rand(size(pos)) - 0.5;
        for ii=1:length(pos)
            x(pos(ii):pos(ii)+width(ii)-1) = x(pos(ii):pos(ii)+width(ii)-1) + height(ii);
        end
        signal = compose_signal(0 * t, x);

    case 5  % audio fajlbol, pl. zenei effekthez
        [signal, fs_file] = audioread('input.wav', 'double');
        if fs_file ~= fs
            fprintf('Figyelem, eltero mintaveteli fekvencia (%d Hz)!\n', fs_file);
        end
        signal = compose_signal(signal(:, 1), signal(:, 2));
end


fid = fopen(filename, 'wt');
fprintf(fid, '%d %d\n', signal');
fclose(fid);
t = (0:size(signal, 1)-1) / fs;
subplot(211)
plot(t, double(signal(:, 1)) / 32768.0)
xlabel('t [s]')
title('Bal csatorna')
subplot(212)
plot(t, double(signal(:, 2)) / 32768.0)
xlabel('t [s]')
title('Jobb csatorna')
