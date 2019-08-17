#!/usr/bin/env python
# coding : utf8

import numpy as np
import matplotlib.pyplot as plt


def ff(inp):
    return np.log10(inp)


def fb(K, ph):
    r = ph / 180.0 * np.pi
    gs = K * np.exp(-1.0j * r)
    ms = gs / (1 + gs)

    z = ff(np.abs(ms))
    r = z
    if z > 40:
        r = 0

    a = 180 * np.pi / np.angle(ms)
    if a < -360 or a > 360:
        a = 0
    return r, a


def to_closed(a, ws):
    ks = []
    phs = []
    for i in range(len(a)):
        k, ph = fb(a[i], ws[i])
        ks.append(k)
        phs.append(ph)
    return ks, phs


def main():
    count = 100
    ws = np.linspace(0, -360, count)

    k = 1e6
    for i in range(10):
        a = k * np.ones(count)
        k /= 100
        plt.plot(ws, to_closed(a, ws)[0], "--")

    plt.grid()
    plt.show()

if __name__ == '__main__':
    main()
