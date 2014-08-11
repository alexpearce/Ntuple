#!/usr/bin/env python

import ROOT

ROOT.gROOT.SetBatch(True)

n = ROOT.TChain("DataTree")
n.Add("data.root")
entries = n.GetEntries()
print "Entries: ", entries

gauss_h = ROOT.TH1F("gauss", "Gaussian distribution", 100, -5., 5.)
uniform_h = ROOT.TH1I("uniform", "Uniform distribution", 100, 0, 101)

for entry in range(entries):
    n.GetEntry(entry)
    gauss_h.Fill(n.gauss_0)
    uniform_h.Fill(n.uniform_0)

canvas = ROOT.TCanvas("c", "c", 400, 400)
gauss_h.Draw()
canvas.SaveAs("gauss.traditional_py.pdf")
canvas.Clear()
uniform_h.Draw()
canvas.SaveAs("uniform.traditional_py.pdf")
