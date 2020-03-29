#include <libff/algebra/curves/bw6_761/bw6_761_g1.hpp>
#include <libff/algebra/curves/bw6_761/bw6_761_g2.hpp>
#include <libff/algebra/curves/bw6_761/bw6_761_init.hpp>

namespace libff {

bigint<bw6_761_r_limbs> bw6_761_modulus_r;
bigint<bw6_761_q_limbs> bw6_761_modulus_q;

bw6_761_Fq bw6_761_coeff_b;
bw6_761_Fq bw6_761_twist;
bw6_761_Fq bw6_761_twist_coeff_b;

bigint<bw6_761_q_limbs> bw6_761_ate_loop_count1;
bigint<bw6_761_q_limbs> bw6_761_ate_loop_count2;
bool bw6_761_ate_is_loop_count_neg;
bigint<bw6_761_q_limbs> bw6_761_final_exponent_z;
bool bw6_761_final_exponent_is_z_neg;

bool bw6_761_D_twist = false; // false: use M-twist

void init_bw6_761_params()
{
    typedef bigint<bw6_761_r_limbs> bigint_r;
    typedef bigint<bw6_761_q_limbs> bigint_q;

    assert(sizeof(mp_limb_t) == 8 || sizeof(mp_limb_t) == 4); // Montgomery assumes this

    /* parameters for scalar field Fr */

    bw6_761_modulus_r = bigint_r("258664426012969094010652733694893533536393512754914660539884262666720468348340822774968888139573360124440321458177");
    assert(bw6_761_Fr::modulus_is_valid());
    if (sizeof(mp_limb_t) == 8)
    {
        bw6_761_Fr::Rsquared = bigint_r("66127428376872697816332570116866232405230528984664918319606315420233909940404532140033099444330447428417853902114"); // Rsquared = (W**k)**2 % r where k=6
        bw6_761_Fr::Rcubed = bigint_r("157734475176213061358192738313701451942220138363611391489992831740412033225490229541667992423878570205050777755168");
        bw6_761_Fr::inv = 0x8508bfffffffffff; // (-1/modulus) mod W
    }
    if (sizeof(mp_limb_t) == 4)
    {
        bw6_761_Fr::Rsquared = bigint_r("66127428376872697816332570116866232405230528984664918319606315420233909940404532140033099444330447428417853902114");
        bw6_761_Fr::Rcubed = bigint_r("157734475176213061358192738313701451942220138363611391489992831740412033225490229541667992423878570205050777755168");
        bw6_761_Fr::inv = 0xffffffff;
    }
    bw6_761_Fr::num_bits = 377;
    bw6_761_Fr::euler = bigint_r("129332213006484547005326366847446766768196756377457330269942131333360234174170411387484444069786680062220160729088");
    bw6_761_Fr::s = 46;
    bw6_761_Fr::t = bigint_r("3675842578061421676390135839012792950148785745837396071634149488243117337281387659330802195819009059");
    bw6_761_Fr::t_minus_1_over_2 = bigint_r("1837921289030710838195067919506396475074392872918698035817074744121558668640693829665401097909504529");
    bw6_761_Fr::multiplicative_generator = bw6_761_Fr("15");
    bw6_761_Fr::root_of_unity = bw6_761_Fr("32863578547254505029601261939868325669770508939375122462904745766352256812585773382134936404344547323199885654433");
    bw6_761_Fr::nqr = bw6_761_Fr("5");
    bw6_761_Fr::nqr_to_t = bw6_761_Fr("33774956008227656219775876656288133547078610493828613777258829345740556592044969439504850374928261397247202212840");

    /* parameters for base field Fq */
    bw6_761_modulus_q = bigint_q("6891450384315732539396789682275657542479668912536150109513790160209623422243491736087683183289411687640864567753786613451161759120554247759349511699125301598951605099378508850372543631423596795951899700429969112842764913119068299");
    assert(bw6_761_Fq::modulus_is_valid());
    if (sizeof(mp_limb_t) == 8)
    {
        bw6_761_Fq::Rsquared = bigint_q("4101737105507298352442561313393192324180371814155294089883586780083371310025435312104187656671185260872966272843049570295923422980866771377818994384387830909209154498924545983803406507410808360495749428678951279422657716620863065");
        bw6_761_Fq::Rcubed = bigint_q("297415514632118086526834853439131925910825545338380841392089322640624198677430665521626961318971291885148318712859327336480040754936765047304839676586453060891603903912680899124818868997690823101153644786484398736894438670450156");

        bw6_761_Fq::inv = 0xa5593568fa798dd;
    }
    if (sizeof(mp_limb_t) == 4)
    {
        bw6_761_Fq::Rsquared = bigint_q("4101737105507298352442561313393192324180371814155294089883586780083371310025435312104187656671185260872966272843049570295923422980866771377818994384387830909209154498924545983803406507410808360495749428678951279422657716620863065");
        bw6_761_Fq::Rcubed = bigint_q("297415514632118086526834853439131925910825545338380841392089322640624198677430665521626961318971291885148318712859327336480040754936765047304839676586453060891603903912680899124818868997690823101153644786484398736894438670450156");
        bw6_761_Fq::inv = 0x8fa798dd;
    }
    bw6_761_Fq::num_bits = 761;
    bw6_761_Fq::euler = bigint_q("3445725192157866269698394841137828771239834456268075054756895080104811711121745868043841591644705843820432283876893306725580879560277123879674755849562650799475802549689254425186271815711798397975949850214984556421382456559534149");
    bw6_761_Fq::s = 1;
    bw6_761_Fq::t = bigint_q("3445725192157866269698394841137828771239834456268075054756895080104811711121745868043841591644705843820432283876893306725580879560277123879674755849562650799475802549689254425186271815711798397975949850214984556421382456559534149");
    bw6_761_Fq::t_minus_1_over_2 = bigint_q("1722862596078933134849197420568914385619917228134037527378447540052405855560872934021920795822352921910216141938446653362790439780138561939837377924781325399737901274844627212593135907855899198987974925107492278210691228279767074");
    bw6_761_Fq::multiplicative_generator = bw6_761_Fq("2");
    bw6_761_Fq::root_of_unity = bw6_761_Fq("6891450384315732539396789682275657542479668912536150109513790160209623422243491736087683183289411687640864567753786613451161759120554247759349511699125301598951605099378508850372543631423596795951899700429969112842764913119068298");
    bw6_761_Fq::nqr = bw6_761_Fq("2");
    bw6_761_Fq::nqr_to_t = bw6_761_Fq("6891450384315732539396789682275657542479668912536150109513790160209623422243491736087683183289411687640864567753786613451161759120554247759349511699125301598951605099378508850372543631423596795951899700429969112842764913119068298");
    bw6_761_Fq::two_inv = (bw6_761_Fq("2").inverse());

    /* parameters for twist field Fq3 */
    bw6_761_Fq3::euler = bigint<3*bw6_761_q_limbs>("163644685426295400324914169940954888955920619637363322051209614326898453970148805686419011719286049425180559084625546731806777278679106679984663099143697116161447047621868304705520956489172590015220902479424161563352136860292139566900393097133389363042061971461347807883724603987331244933262704461518541983713459799376468603514421848466717682565239322945704382553665794724881767934477627356011124130848888786345848916593956748157692324278157203986274300190783511909765368289882962935850660388936997934686011687910278075743164270849285352627899889251291640002032048904566729137025370148302740362549750397891505412715221417910833483274288268810463328915955262815220574933205354950574767449");
    bw6_761_Fq3::s = 1;
    bw6_761_Fq3::t = bigint<3*bw6_761_q_limbs>("163644685426295400324914169940954888955920619637363322051209614326898453970148805686419011719286049425180559084625546731806777278679106679984663099143697116161447047621868304705520956489172590015220902479424161563352136860292139566900393097133389363042061971461347807883724603987331244933262704461518541983713459799376468603514421848466717682565239322945704382553665794724881767934477627356011124130848888786345848916593956748157692324278157203986274300190783511909765368289882962935850660388936997934686011687910278075743164270849285352627899889251291640002032048904566729137025370148302740362549750397891505412715221417910833483274288268810463328915955262815220574933205354950574767449");
    bw6_761_Fq3::t_minus_1_over_2 = bigint<3*bw6_761_q_limbs>("81822342713147700162457084970477444477960309818681661025604807163449226985074402843209505859643024712590279542312773365903388639339553339992331549571848558080723523810934152352760478244586295007610451239712080781676068430146069783450196548566694681521030985730673903941862301993665622466631352230759270991856729899688234301757210924233358841282619661472852191276832897362440883967238813678005562065424444393172924458296978374078846162139078601993137150095391755954882684144941481467925330194468498967343005843955139037871582135424642676313949944625645820001016024452283364568512685074151370181274875198945752706357610708955416741637144134405231664457977631407610287466602677475287383724");
    bw6_761_Fq3::nqr = bw6_761_Fq3(bw6_761_Fq("0"),bw6_761_Fq("1"),bw6_761_Fq("0"));
    bw6_761_Fq3::nqr_to_t = bw6_761_Fq3(bw6_761_Fq("6891450384315732539396789682275657542479668912536150109513790160209623422243491736087683183289411687640864567753786613451161759120554247759349511699125301598951605099378508850372543631423596795951899700429969112842764913119068298"),bw6_761_Fq("0"),bw6_761_Fq("0"));
    if(bw6_761_D_twist)
    {
      bw6_761_Fq3::non_residue = bw6_761_Fq("2");
      bw6_761_Fq3::Frobenius_coeffs_c1[0] = bw6_761_Fq("1");
      bw6_761_Fq3::Frobenius_coeffs_c1[1] = bw6_761_Fq("1968985824090209297278610739700577151397666382303825728450741611566800370218827257750865013421937292370006175842381275743914023380727582819905021229583192207421122272650305267822868639090213645505120388400344940985710520836292650");
      bw6_761_Fq3::Frobenius_coeffs_c1[2] = bw6_761_Fq("4922464560225523242118178942575080391082002530232324381063048548642823052024664478336818169867474395270858391911405337707247735739826664939444490469542109391530482826728203582549674992333383150446779312029624171857054392282775648");
      bw6_761_Fq3::Frobenius_coeffs_c2[0] = bw6_761_Fq("1");
      bw6_761_Fq3::Frobenius_coeffs_c2[1] = bw6_761_Fq("4922464560225523242118178942575080391082002530232324381063048548642823052024664478336818169867474395270858391911405337707247735739826664939444490469542109391530482826728203582549674992333383150446779312029624171857054392282775648");
      bw6_761_Fq3::Frobenius_coeffs_c2[2] = bw6_761_Fq("1968985824090209297278610739700577151397666382303825728450741611566800370218827257750865013421937292370006175842381275743914023380727582819905021229583192207421122272650305267822868639090213645505120388400344940985710520836292650");
    }
    else // M-twist
    {
      bw6_761_Fq3::non_residue = bw6_761_Fq("6891450384315732539396789682275657542479668912536150109513790160209623422243491736087683183289411687640864567753786613451161759120554247759349511699125301598951605099378508850372543631423596795951899700429969112842764913119068295");
      bw6_761_Fq3::nqr = bw6_761_Fq3(bw6_761_Fq("0"),bw6_761_Fq("1"),bw6_761_Fq("0"));
      bw6_761_Fq3::nqr_to_t = bw6_761_Fq3(bw6_761_Fq("6891450384315732539396789682275657542479668912536150109513790160209623422243491736087683183289411687640864567753786613451161759120554247759349511699125301598951605099378508850372543631423596795951899700429969112842764913119068298"),bw6_761_Fq("0"),bw6_761_Fq("0"));
      bw6_761_Fq3::Frobenius_coeffs_c1[0] = bw6_761_Fq("1");
      bw6_761_Fq3::Frobenius_coeffs_c1[1] = bw6_761_Fq("4922464560225523242118178942575080391082002530232324381063048548642823052024664478336818169867474395270858391911405337707247735739826664939444490469542109391530482826728203582549674992333383150446779312029624171857054392282775648");
      bw6_761_Fq3::Frobenius_coeffs_c1[2] = bw6_761_Fq("1968985824090209297278610739700577151397666382303825728450741611566800370218827257750865013421937292370006175842381275743914023380727582819905021229583192207421122272650305267822868639090213645505120388400344940985710520836292650");
      bw6_761_Fq3::Frobenius_coeffs_c2[0] = bw6_761_Fq("1");
      bw6_761_Fq3::Frobenius_coeffs_c2[1] = bw6_761_Fq("1968985824090209297278610739700577151397666382303825728450741611566800370218827257750865013421937292370006175842381275743914023380727582819905021229583192207421122272650305267822868639090213645505120388400344940985710520836292650");
      bw6_761_Fq3::Frobenius_coeffs_c2[2] = bw6_761_Fq("4922464560225523242118178942575080391082002530232324381063048548642823052024664478336818169867474395270858391911405337707247735739826664939444490469542109391530482826728203582549674992333383150446779312029624171857054392282775648");
    }

    /* parameters for Fq6 */
    if (bw6_761_D_twist)
    {
      bw6_761_Fq6::non_residue = bw6_761_Fq("2");
      bw6_761_Fq6::Frobenius_coeffs_c1[0] = bw6_761_Fq("1");
      bw6_761_Fq6::Frobenius_coeffs_c1[1] = bw6_761_Fq("1968985824090209297278610739700577151397666382303825728450741611566800370218827257750865013421937292370006175842381275743914023380727582819905021229583192207421122272650305267822868639090213645505120388400344940985710520836292651");
      bw6_761_Fq6::Frobenius_coeffs_c1[2] = bw6_761_Fq("1968985824090209297278610739700577151397666382303825728450741611566800370218827257750865013421937292370006175842381275743914023380727582819905021229583192207421122272650305267822868639090213645505120388400344940985710520836292650");
      bw6_761_Fq6::Frobenius_coeffs_c1[3] = bw6_761_Fq("6891450384315732539396789682275657542479668912536150109513790160209623422243491736087683183289411687640864567753786613451161759120554247759349511699125301598951605099378508850372543631423596795951899700429969112842764913119068298");
      bw6_761_Fq6::Frobenius_coeffs_c1[4] = bw6_761_Fq("4922464560225523242118178942575080391082002530232324381063048548642823052024664478336818169867474395270858391911405337707247735739826664939444490469542109391530482826728203582549674992333383150446779312029624171857054392282775648");
      bw6_761_Fq6::Frobenius_coeffs_c1[5] = bw6_761_Fq("4922464560225523242118178942575080391082002530232324381063048548642823052024664478336818169867474395270858391911405337707247735739826664939444490469542109391530482826728203582549674992333383150446779312029624171857054392282775649");
    }
    else
    {
      bw6_761_Fq6::non_residue = bw6_761_Fq("6891450384315732539396789682275657542479668912536150109513790160209623422243491736087683183289411687640864567753786613451161759120554247759349511699125301598951605099378508850372543631423596795951899700429969112842764913119068295");
      bw6_761_Fq6::Frobenius_coeffs_c1[0] = bw6_761_Fq("1");
      bw6_761_Fq6::Frobenius_coeffs_c1[1] = bw6_761_Fq("4922464560225523242118178942575080391082002530232324381063048548642823052024664478336818169867474395270858391911405337707247735739826664939444490469542109391530482826728203582549674992333383150446779312029624171857054392282775649");
      bw6_761_Fq6::Frobenius_coeffs_c1[2] = bw6_761_Fq("4922464560225523242118178942575080391082002530232324381063048548642823052024664478336818169867474395270858391911405337707247735739826664939444490469542109391530482826728203582549674992333383150446779312029624171857054392282775648");
      bw6_761_Fq6::Frobenius_coeffs_c1[3] = bw6_761_Fq("6891450384315732539396789682275657542479668912536150109513790160209623422243491736087683183289411687640864567753786613451161759120554247759349511699125301598951605099378508850372543631423596795951899700429969112842764913119068298");
      bw6_761_Fq6::Frobenius_coeffs_c1[4] = bw6_761_Fq("1968985824090209297278610739700577151397666382303825728450741611566800370218827257750865013421937292370006175842381275743914023380727582819905021229583192207421122272650305267822868639090213645505120388400344940985710520836292650");
      bw6_761_Fq6::Frobenius_coeffs_c1[5] = bw6_761_Fq("1968985824090209297278610739700577151397666382303825728450741611566800370218827257750865013421937292370006175842381275743914023380727582819905021229583192207421122272650305267822868639090213645505120388400344940985710520836292651");
    }

    bw6_761_Fq6::my_Fp2::non_residue = bw6_761_Fq3::non_residue;

    /* choice of short Weierstrass curve and its twist */
    bw6_761_coeff_b = bw6_761_Fq("6891450384315732539396789682275657542479668912536150109513790160209623422243491736087683183289411687640864567753786613451161759120554247759349511699125301598951605099378508850372543631423596795951899700429969112842764913119068298");
    bw6_761_G1::coeff_b = bw6_761_coeff_b;
    if(bw6_761_D_twist)
    {
      bw6_761_twist = bw6_761_Fq("2");
      // bw6_761_twist_coeff_b = bw6_761_coeff_b * bw6_761_twist.inverse();
      bw6_761_twist_coeff_b = bw6_761_Fq("3445725192157866269698394841137828771239834456268075054756895080104811711121745868043841591644705843820432283876893306725580879560277123879674755849562650799475802549689254425186271815711798397975949850214984556421382456559534149");
    }
    else
    {
      bw6_761_twist = bw6_761_Fq("6891450384315732539396789682275657542479668912536150109513790160209623422243491736087683183289411687640864567753786613451161759120554247759349511699125301598951605099378508850372543631423596795951899700429969112842764913119068295");
      // bw6_761_twist_coeff_b = bw6_761_coeff_b * bw6_761_twist;
      bw6_761_twist_coeff_b = bw6_761_Fq("4");
    }
    bw6_761_G2::twist = bw6_761_twist;
    bw6_761_G2::coeff_b = bw6_761_twist_coeff_b;

    /* choice of group G1 */
    bw6_761_G1::G1_zero = bw6_761_G1(bw6_761_Fq::zero(),
                             bw6_761_Fq::one(),
                             bw6_761_Fq::zero());
    bw6_761_G1::G1_one = bw6_761_G1(bw6_761_Fq("6238772257594679368032145693622812838779005809760824733138787810501188623461307351759238099287535516224314149266511977132140828635950940021790489507611754366317801811090811367945064510304504157188661901055903167026722666149426237"),
                                    bw6_761_Fq("2101735126520897423911504562215834951148127555913367997162789335052900271653517958562461315794228241561913734371411178226936527683203879553093934185950470971848972085321797958124416462268292467002957525517188485984766314758624099"),
                                    bw6_761_Fq::one());

    // TODO: wNAF window table
    bw6_761_G1::wnaf_window_table.resize(0);
    bw6_761_G1::wnaf_window_table.push_back(11);
    bw6_761_G1::wnaf_window_table.push_back(24);
    bw6_761_G1::wnaf_window_table.push_back(60);
    bw6_761_G1::wnaf_window_table.push_back(127);

    // TODO: fixed-base exponentiation table
    bw6_761_G1::fixed_base_exp_window_table.resize(0);
    // window 1 is unbeaten in [-inf, 4.99]
    bw6_761_G1::fixed_base_exp_window_table.push_back(1);
    // window 2 is unbeaten in [4.99, 10.99]
    bw6_761_G1::fixed_base_exp_window_table.push_back(5);
    // window 3 is unbeaten in [10.99, 32.29]
    bw6_761_G1::fixed_base_exp_window_table.push_back(11);
    // window 4 is unbeaten in [32.29, 55.23]
    bw6_761_G1::fixed_base_exp_window_table.push_back(32);
    // window 5 is unbeaten in [55.23, 162.03]
    bw6_761_G1::fixed_base_exp_window_table.push_back(55);
    // window 6 is unbeaten in [162.03, 360.15]
    bw6_761_G1::fixed_base_exp_window_table.push_back(162);
    // window 7 is unbeaten in [360.15, 815.44]
    bw6_761_G1::fixed_base_exp_window_table.push_back(360);
    // window 8 is unbeaten in [815.44, 2373.07]
    bw6_761_G1::fixed_base_exp_window_table.push_back(815);
    // window 9 is unbeaten in [2373.07, 6977.75]
    bw6_761_G1::fixed_base_exp_window_table.push_back(2373);
    // window 10 is unbeaten in [6977.75, 7122.23]
    bw6_761_G1::fixed_base_exp_window_table.push_back(6978);
    // window 11 is unbeaten in [7122.23, 57818.46]
    bw6_761_G1::fixed_base_exp_window_table.push_back(7122);
    // window 12 is never the best
    bw6_761_G1::fixed_base_exp_window_table.push_back(0);
    // window 13 is unbeaten in [57818.46, 169679.14]
    bw6_761_G1::fixed_base_exp_window_table.push_back(57818);
    // window 14 is never the best
    bw6_761_G1::fixed_base_exp_window_table.push_back(0);
    // window 15 is unbeaten in [169679.14, 439758.91]
    bw6_761_G1::fixed_base_exp_window_table.push_back(169679);
    // window 16 is unbeaten in [439758.91, 936073.41]
    bw6_761_G1::fixed_base_exp_window_table.push_back(439759);
    // window 17 is unbeaten in [936073.41, 4666554.74]
    bw6_761_G1::fixed_base_exp_window_table.push_back(936073);
    // window 18 is never the best
    bw6_761_G1::fixed_base_exp_window_table.push_back(0);
    // window 19 is unbeaten in [4666554.74, 7580404.42]
    bw6_761_G1::fixed_base_exp_window_table.push_back(4666555);
    // window 20 is unbeaten in [7580404.42, 34552892.20]
    bw6_761_G1::fixed_base_exp_window_table.push_back(7580404);
    // window 21 is never the best
    bw6_761_G1::fixed_base_exp_window_table.push_back(0);
    // window 22 is unbeaten in [34552892.20, inf]
    bw6_761_G1::fixed_base_exp_window_table.push_back(34552892);


    /* choice of group G2 */
    bw6_761_G2::G2_zero = bw6_761_G2(bw6_761_Fq::zero(),
                             bw6_761_Fq::one(),
                             bw6_761_Fq::zero());

    if(bw6_761_D_twist)
    {
      bw6_761_G2::G2_one = bw6_761_G2(bw6_761_Fq("6262099733932320299029659201229457369633385349245308649169574015486237715338854821227744078654927847539887539929071093904227797391629046445965324190244834384691163899627970142994762818399587817499184262597778764236207145726130393"),
                              bw6_761_Fq("2534560989068955423315594688592085176389251018664186742606195596149391318197797112536948759146576652843716851108848083042375238058152456021190084249045595611396757523591848210543808321850458423117571529414585724012999919622918176"),

                              bw6_761_Fq::one());
    }
    else
    {
      bw6_761_G2::G2_one = bw6_761_G2(bw6_761_Fq("6445332910596979336035888152774071626898886139774101364933948236926875073754470830732273879639675437155036544153105017729592600560631678554299562762294743927912429096636156401171909259073181112518725201388196280039960074422214428"),
                              bw6_761_Fq("562923658089539719386922163444547387757586534741080263946953401595155211934630598999300396317104182598044793758153214972605680357108252243146746187917218885078195819486220416605630144001533548163105316661692978285266378674355041"),
                              bw6_761_Fq::one());

    }


    // TODO: wNAF window table
    bw6_761_G2::wnaf_window_table.resize(0);
    bw6_761_G2::wnaf_window_table.push_back(5);
    bw6_761_G2::wnaf_window_table.push_back(15);
    bw6_761_G2::wnaf_window_table.push_back(39);
    bw6_761_G2::wnaf_window_table.push_back(109);

    // TODO: fixed-base exponentiation table
    bw6_761_G2::fixed_base_exp_window_table.resize(0);
    // window 1 is unbeaten in [-inf, 5.10]
    bw6_761_G2::fixed_base_exp_window_table.push_back(1);
    // window 2 is unbeaten in [5.10, 10.43]
    bw6_761_G2::fixed_base_exp_window_table.push_back(5);
    // window 3 is unbeaten in [10.43, 25.28]
    bw6_761_G2::fixed_base_exp_window_table.push_back(10);
    // window 4 is unbeaten in [25.28, 59.00]
    bw6_761_G2::fixed_base_exp_window_table.push_back(25);
    // window 5 is unbeaten in [59.00, 154.03]
    bw6_761_G2::fixed_base_exp_window_table.push_back(59);
    // window 6 is unbeaten in [154.03, 334.25]
    bw6_761_G2::fixed_base_exp_window_table.push_back(154);
    // window 7 is unbeaten in [334.25, 742.58]
    bw6_761_G2::fixed_base_exp_window_table.push_back(334);
    // window 8 is unbeaten in [742.58, 2034.40]
    bw6_761_G2::fixed_base_exp_window_table.push_back(743);
    // window 9 is unbeaten in [2034.40, 4987.56]
    bw6_761_G2::fixed_base_exp_window_table.push_back(2034);
    // window 10 is unbeaten in [4987.56, 8888.27]
    bw6_761_G2::fixed_base_exp_window_table.push_back(4988);
    // window 11 is unbeaten in [8888.27, 26271.13]
    bw6_761_G2::fixed_base_exp_window_table.push_back(8888);
    // window 12 is unbeaten in [26271.13, 39768.20]
    bw6_761_G2::fixed_base_exp_window_table.push_back(26271);
    // window 13 is unbeaten in [39768.20, 106275.75]
    bw6_761_G2::fixed_base_exp_window_table.push_back(39768);
    // window 14 is unbeaten in [106275.75, 141703.40]
    bw6_761_G2::fixed_base_exp_window_table.push_back(106276);
    // window 15 is unbeaten in [141703.40, 462422.97]
    bw6_761_G2::fixed_base_exp_window_table.push_back(141703);
    // window 16 is unbeaten in [462422.97, 926871.84]
    bw6_761_G2::fixed_base_exp_window_table.push_back(462423);
    // window 17 is unbeaten in [926871.84, 4873049.17]
    bw6_761_G2::fixed_base_exp_window_table.push_back(926872);
    // window 18 is never the best
    bw6_761_G2::fixed_base_exp_window_table.push_back(0);
    // window 19 is unbeaten in [4873049.17, 5706707.88]
    bw6_761_G2::fixed_base_exp_window_table.push_back(4873049);
    // window 20 is unbeaten in [5706707.88, 31673814.95]
    bw6_761_G2::fixed_base_exp_window_table.push_back(5706708);
    // window 21 is never the best
    bw6_761_G2::fixed_base_exp_window_table.push_back(0);
    // window 22 is unbeaten in [31673814.95, inf]
    bw6_761_G2::fixed_base_exp_window_table.push_back(31673815);



    /* pairing parameters */
    // z+1
    bw6_761_ate_loop_count1 = bigint_q("9586122913090633730");
    // z^3-z^2-z
    bw6_761_ate_loop_count2 = bigint_q("880904806456922042166256752416502360955572640081583800319");
    bw6_761_ate_is_loop_count_neg = false;
    bw6_761_final_exponent_z = bigint_q("9586122913090633729");
    bw6_761_final_exponent_is_z_neg = false;

}
} // libff
