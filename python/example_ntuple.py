#!/usr/bin/env python

import ROOT

from Ntuple import Ntuple

ROOT.gROOT.SetBatch(True)

n = Ntuple("DataTree")
n.add("data.root")
entries = n.GetEntries()
print "Entries: ", entries

gauss_h = ROOT.TH1F("gauss", "Gaussian distribution", 100, -5., 5.)
uniform_h = ROOT.TH1I("uniform", "Uniform distribution", 101, 0, 101)

for entry in range(entries):
    n.GetEntry(entry)
    gauss_h.Fill(n.val("gauss_0"))
    uniform_h.Fill(n.val("uniform_0"))

canvas = ROOT.TCanvas("c", "c", 400, 400)
gauss_h.Draw()
canvas.SaveAs("gauss.ntuple_py.pdf")
canvas.Clear()
uniform_h.Draw()
canvas.SaveAs("uniform.ntuple_py.pdf")
