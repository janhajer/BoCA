#include "standardmodel/analysis/StandardModel.hh"
#include "boca/generic/DEBUG_MACROS.hh"

namespace standardmodel
{

namespace analysis
{

std::string ProcessName(Process process)
{
    switch (process) {
    case Process::tt : return "tt";
    case Process::tt_lep : return "tt_leptonic";
    case Process::tt_had : return "tt_hadronic";
    case Process::bb : return "bb";
    case Process::cc : return "cc";
    case Process::qq : return "qq";
    case Process::gg : return "gg";
    case Process::hh : return "hh";
    case Process::hh_bb : return "hh_bb";
    case Process::ww : return "ww";
    case Process::zz : return "zz";
    case Process::zz_bb : return "zz_bb";
        DEFAULT(to_int(process), "");
    }
}

latex::String LatexName(Process process)
{
    switch (process) {
    case Process::bb : return {"b", true};
    case Process::cc : return {"c", true};
    case Process::qq : return {"q", true};
    case Process::gg : return {"g", true};
    case Process::hh : return {"h", true};
    case Process::hh_bb : return "h#lbar_{b}";
    case Process::ww : return "W^{#pm}";
    case Process::zz : return {"Z", true};
    case Process::zz_bb : return "Z#lbar_{b}";
    case Process::tt_had : return "t_{h}";
    case Process::tt_lep : return "t_{l}";
        DEFAULT(ProcessName(process), "");
    }
}

std::string Name(Process process)
{
    switch (process) {
    case Process::bb : return "bottom";
    case Process::cc : return "charm";
    case Process::qq : return "quark";
    case Process::gg : return "gluon";
    case Process::hh : return "Higgs";
    case Process::hh_bb : return "Higgs";
    case Process::ww : return "W";
    case Process::zz : return "Z";
    case Process::zz_bb : return "Z";
    case Process::tt_had : return "top";
    case Process::tt_lep : return "top";
        DEFAULT(ProcessName(process), "");
    }
}

}

}
