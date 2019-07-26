/**
 *****************************************************************************
 * @author     This file is part of libff, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/
#include <iostream>
#include <sstream>
#include <libff/algebra/curves/edwards/edwards_pp.hpp>
#include <libff/algebra/curves/mnt/mnt4/mnt4_pp.hpp>
#include <libff/algebra/curves/mnt/mnt6/mnt6_pp.hpp>
#include <libff/common/profiling.hpp>
#ifdef CURVE_BN128
#include <libff/algebra/curves/bn128/bn128_pp.hpp>
#endif
#include <libff/algebra/curves/alt_bn128/alt_bn128_pp.hpp>

#include <libff/algebra/curves/bls12_377/bls12_377_pp.hpp>
#include <libff/algebra/curves/sw6/sw6_pp.hpp>
#include <libff/algebra/curves/bls12_381/bls12_381_pp.hpp>

using namespace libff;

template<typename GroupT>
void test_mixed_add()
{
    GroupT base, el, result;

    base = GroupT::zero();
    el = GroupT::zero();
    el.to_special();
    result = base.mixed_add(el);
    assert(result == base + el);

    base = GroupT::zero();
    el = GroupT::random_element();
    el.to_special();
    result = base.mixed_add(el);
    assert(result == base + el);

    base = GroupT::random_element();
    el = GroupT::zero();
    el.to_special();
    result = base.mixed_add(el);
    assert(result == base + el);

    base = GroupT::random_element();
    el = GroupT::random_element();
    el.to_special();
    result = base.mixed_add(el);
    assert(result == base + el);

    base = GroupT::random_element();
    el = base;
    el.to_special();
    result = base.mixed_add(el);
    assert(result == base.dbl());
}

template<typename GroupT>
void test_group()
{
    bigint<1> rand1 = bigint<1>("76749407");
    bigint<1> rand2 = bigint<1>("44410867");
    bigint<1> randsum = bigint<1>("121160274");

    GroupT zero = GroupT::zero();
    assert(zero == zero);
    GroupT one = GroupT::one();
    assert(one == one);
    GroupT two = bigint<1>(2l) * GroupT::one();
    assert(two == two);
    GroupT five = bigint<1>(5l) * GroupT::one();

    GroupT three = bigint<1>(3l) * GroupT::one();
    GroupT four = bigint<1>(4l) * GroupT::one();

    assert(two+five == three+four);

    GroupT a = GroupT::random_element();
    // std::cout << "random point 1:\n";
    // a.print_coordinates();
    GroupT b = GroupT::random_element();
    // std::cout << "random point:\n";
    // b.print_coordinates();

    assert(one != zero);
    assert(a != zero);
    assert(a != one);

    assert(b != zero);
    assert(b != one);

    assert(a.dbl() == a + a);
    assert(b.dbl() == b + b);
    assert(one.add(two) == three);
    assert(two.add(one) == three);
    assert(a + b == b + a);
    assert(a - a == zero);
    assert(a - b == a + (-b));
    assert(a - b == (-b) + a);

    // handle special cases
    assert(zero + (-a) == -a);
    assert(zero - a == -a);
    assert(a - zero == a);
    assert(a + zero == a);
    assert(zero + a == a);

    assert((a + b).dbl() == (a + b) + (b + a));
    assert(bigint<1>("2") * (a + b) == (a + b) + (b + a));

    assert((rand1 * a) + (rand2 * a) == (randsum * a));

    assert(GroupT::order() * a == zero);
    assert(GroupT::order() * one == zero);
    assert((GroupT::order() * a) - a != zero);
    assert((GroupT::order() * one) - one != zero);

    test_mixed_add<GroupT>();
}

template<typename GroupT>
void test_mul_by_q()
{
    GroupT a = GroupT::random_element();
    // GroupT tmp = a.mul_by_q();
    // std::cout << "get X.Frobenius_map(1) and Y.Frobenius_map(1)\n";
    // tmp.print();
    assert((GroupT::base_field_char()*a) == a.mul_by_q());
}

template<typename GroupT>
void test_output()
{
    GroupT g = GroupT::one();

    for (size_t i = 0; i < 100; ++i)
    {
        std::stringstream ss;
        ss << g;
        GroupT gg;
        ss >> gg;
        assert(g == gg);
        /* use a random point in next iteration */
        g = GroupT::random_element();
    }
}

int main(void)
{
    // edwards_pp::init_public_params();
    // test_group<G1<edwards_pp> >();
    // test_output<G1<edwards_pp> >();
    // test_group<G2<edwards_pp> >();
    // test_output<G2<edwards_pp> >();
    // test_mul_by_q<G2<edwards_pp> >();

    // mnt4_pp::init_public_params();
    // test_group<G1<mnt4_pp> >();
    // test_output<G1<mnt4_pp> >();
    // test_group<G2<mnt4_pp> >();
    // test_output<G2<mnt4_pp> >();
    // test_mul_by_q<G2<mnt4_pp> >();

    // mnt6_pp::init_public_params();
    // test_group<G1<mnt6_pp> >();
    // test_output<G1<mnt6_pp> >();
    // test_group<G2<mnt6_pp> >();
    // test_output<G2<mnt6_pp> >();
    // test_mul_by_q<G2<mnt6_pp> >();

    // alt_bn128_pp::init_public_params();
    // test_group<G1<alt_bn128_pp> >();
    // test_output<G1<alt_bn128_pp> >();
    // test_group<G2<alt_bn128_pp> >();
    // test_output<G2<alt_bn128_pp> >();
    // test_mul_by_q<G2<alt_bn128_pp> >();

    // new curve: BLS12_377
    // bls12_377_pp::init_public_params();
    // test_group<G1<bls12_377_pp> >();
    // test_output<G1<bls12_377_pp> >();
    // test_group<G2<bls12_377_pp> >();
    // test_output<G2<bls12_377_pp> >();
    // test_mul_by_q<G2<bls12_377_pp> >();
    
    // new curve: SW6
    // sw6_pp::init_public_params();
    // test_group<G1<sw6_pp> >();
    // test_output<G1<sw6_pp> >();
    // test_group<G2<sw6_pp> >();
    // test_output<G2<sw6_pp> >();
    // test_mul_by_q<G2<sw6_pp> >();
    
    // new curve: BLS12_377
    bls12_377_pp::init_public_params();
    test_group<G1<bls12_377_pp> >();
    test_output<G1<bls12_377_pp> >();
    test_group<G2<bls12_377_pp> >();
    test_output<G2<bls12_377_pp> >();
    test_mul_by_q<G2<bls12_377_pp> >();
    
#ifdef CURVE_BN128       // BN128 has fancy dependencies so it may be disabled
    // bn128_pp::init_public_params();
    // test_group<G1<bn128_pp> >();
    // test_output<G1<bn128_pp> >();
    // test_group<G2<bn128_pp> >();
    // test_output<G2<bn128_pp> >();
#endif
}
