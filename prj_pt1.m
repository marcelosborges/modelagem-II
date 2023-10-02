clc;
clear all;
close all;

%% Input data
data = readtable('interpdata.csv');
u1 = data.PWM;
y1 = data.Temp;
u1m = u1-mean(u1);
y1m = y1-mean(y1);

%% Treat data
dataset = iddata(y1m,u1m,0.2);
figure(1);
plot(dataset)

training = dataset(1:400);
validation = dataset(401:end);

%% Model prediction
coef = [];
ajustes = [];

opt = arxOptions('Focus','simulation');

for i = 1:5
    for j = 1:5
        for k = 1:3
            coef = [coef; [i, j, k]];
            Mtest = arx(training,[i j k], opt);
            [a b] = compare(Mtest, validation);
            ajustes = [ajustes; b];
        end
    end
end

data = table(coef, ajustes, 'VariableNames', {'coef', 'ajuste'});
sorted_data = sortrows(data, 'ajuste', 'descend');
fprintf("Os melhores coeficientes foram: ")
best_coef = sorted_data.coef(1,:)

fprintf("Com o ajuste de: ")
best_adj = sorted_data.ajuste(1)
M1 = arx(training,best_coef, opt)
figure(2)
compare(M1, validation)