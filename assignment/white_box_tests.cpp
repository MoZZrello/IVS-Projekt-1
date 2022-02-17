//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     Richard Harman <xharma05@stud.fit.vutbr.cz>
// $Date:       $2022-02-17
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Richard Harman
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

class UndefinedMatrix : public ::testing::Test{
protected:
    Matrix *m;
    Matrix *m2;
    const int row = 3;
    const int column = 4;
};

class DefMatrix : public ::testing::Test{
protected:
    Matrix *m;
    const int row = 2;
    const int column = 2;
    const std::vector<std::vector<double>> values = {
            { 8.1, 1.6 },
            { 2.1, 2.0 }
    };

    virtual void SetUp(){
        m = new Matrix(row, column);
        m->set(values);
    }

    virtual void TearDown(){
        delete m;
    }
};

/*
 * Inicializacia matice 1x1
 */
TEST_F(UndefinedMatrix, Construct1x1){
    EXPECT_NO_THROW(m = new Matrix(1, 1));
    EXPECT_NO_THROW(m->set({{0}}));
    delete m;
}

/*
 * 1. Test inicializacie matice pre velkosti 0x0 ; 1x0 ; 0x1
 * 2. Inicializacia matice s presnou velkostou (R,C)
 */
TEST_F(UndefinedMatrix, Construct){
    m = nullptr;
//-------------------1.----------------------
    EXPECT_ANY_THROW(m = new Matrix(0, 0));
    delete m;
    m = nullptr;

    EXPECT_ANY_THROW(m = new Matrix(1, 0));
    delete m;
    m = nullptr;

    EXPECT_ANY_THROW(m = new Matrix(0, 1));
    delete m;
    m = nullptr;
//-------------------2.----------------------
    EXPECT_NO_THROW(m = new Matrix(row, column));
    EXPECT_NE(m, nullptr);
    delete m;
}

/*
 * Test inverznej matice.
 * 1. Inverzna so zlymi rozmermi (ocakavame chybu)
 * 2. Inverzna 2x2
 * 3. Inverzna 3x3
 */
TEST_F(UndefinedMatrix, InverseMatrix){
    Matrix *m1, *m2x2, *m3x3, res;
    Matrix *i2x2 = new Matrix(2, 2);
    Matrix *i3x3 = new Matrix(3, 3);
    i2x2->set({ {1, 0},
                {0, 1} });
    i3x3->set({{ 1, 0, 0 },
               { 0, 1, 0 },
               { 0, 0, 1 }});
//-------------------1.----------------------
    m1 = new Matrix(2,3);
    EXPECT_ANY_THROW(m1->inverse());
    delete m1;
//-------------------2.----------------------
    m2x2 = new Matrix(2,2);
    for(int r = 0; r < 2; r++){
        EXPECT_NO_THROW(m2x2->set(r, 0, r+1));
    }
    EXPECT_ANY_THROW(m2x2->inverse());
    for(int r = 0; r < 2; r++){
        EXPECT_NO_THROW(m2x2->set(r, 1, r+2));
    }
    EXPECT_NO_THROW(res = m2x2->inverse());
    EXPECT_TRUE(*i2x2 == (res * *m2x2));
    EXPECT_TRUE(*i2x2 == (*m2x2 * res));
//-------------------3.----------------------
    m3x3 = new Matrix(3,3);
    for(int r = 0; r < 3; r++){
        for(int c = 0; c < 2; c++){
            EXPECT_NO_THROW(m3x3->set(r,c, r+1-c));
        }
    }
    EXPECT_NO_THROW(m3x3->set(2,0,-3));
    EXPECT_ANY_THROW(m3x3->inverse());
    EXPECT_NO_THROW(m3x3->set(2,2,1));
    EXPECT_NO_THROW(res = m3x3->inverse());
    EXPECT_TRUE(*i3x3 == (res * *m3x3));
    EXPECT_TRUE(*i3x3 == (*m3x3 * res));
    delete i2x2;
    delete i3x3;
    delete m2x2;
    delete m3x3;
}

/*
 * 1. Spravne sustavy rovnic na vypocet
 * 2. Determinant sa rovna 0
 * 3. Matica nie je stvorecova
 */
TEST_F(DefMatrix, SolveEquation){
    std::vector<double> temp0 = { 1, 1 }, temp1 = { 8 }, temp2 = { 4, 3, 10 }, temp3 = { 2, 5, 21, 1 };
    std::vector<double> result1 = {1.6}, result2 = {-0.5,0.5};
//-------------------1.----------------------
    Matrix *testMatrix1x1 = new Matrix(1,1);
    testMatrix1x1->set(std::vector<std::vector<double>>{{5}});
    Matrix *testMatrix2x2 = new Matrix(2,2);
    testMatrix2x2->set(std::vector<std::vector<double>>{{1,3},{5,7}});
    EXPECT_EQ(testMatrix1x1->solveEquation(temp1),result1);
    EXPECT_EQ(testMatrix2x2->solveEquation(temp0),result2);
//-------------------2.----------------------
    testMatrix1x1->set(std::vector<std::vector<double>>{{0}});
    testMatrix2x2->set(std::vector<std::vector<double>>{{1,1},{1,1}});
    Matrix *testMatrix3x3 = new Matrix(3,3);
    testMatrix3x3->set(std::vector<std::vector<double>>{{1,3,5},{5,7,9},{2,4,6}});
    Matrix *testMatrix4x4 = new Matrix(4,4);
    testMatrix4x4->set(std::vector<std::vector<double>>{{1,3,5,7},{9,10,11,12},{2,4,6,8},{13,14,15,16}});
    EXPECT_ANY_THROW(testMatrix1x1->solveEquation(temp1));
    EXPECT_ANY_THROW(testMatrix2x2->solveEquation(temp0));
    EXPECT_ANY_THROW(testMatrix3x3->solveEquation(temp2));
    EXPECT_ANY_THROW(testMatrix4x4->solveEquation(temp3));
//-------------------3.----------------------
    Matrix *testMatrix1x3 = new Matrix(1,3);
    testMatrix1x3->set(std::vector<std::vector<double>>{{1,3,5}});
    Matrix *testMatrix4x2 = new Matrix(4,2);
    testMatrix4x2->set(std::vector<std::vector<double>>{{1,3},{5,7},{9,11},{2,4}});
    EXPECT_ANY_THROW(testMatrix1x3->solveEquation(std::vector<double>{1}));
    EXPECT_ANY_THROW(testMatrix1x3->solveEquation(std::vector<double>{1,2,3}));
    EXPECT_ANY_THROW(testMatrix4x2->solveEquation(std::vector<double>{1,2}));
    EXPECT_ANY_THROW(testMatrix4x2->solveEquation(std::vector<double>{1,2,3,4}));
}

/*
 * Vkladam po jednom prvku.
 * Ak je to vo velkosti danej matice, ocakavam TRUE.
 * Ak vkladam mimo matice, ocakavam FALSE.
 * Test spraveny na maximalnu velkost matice 5x5
 */
TEST_F(DefMatrix, SetValue_Single){
    ASSERT_NE(m, nullptr);
    int value = 69;
    for(int r = 0; r < 6; r++){
        for(int c = 0; c < 6; c++){
            if(r < row && c < column){
                EXPECT_TRUE(m->set(r, c, value));
            }else{
                EXPECT_FALSE(m->set(r, c, value));
            }
        }
    }
}

/*
 * Do inicializovanej matice 2x2 vkladam matice o velkosti:
 * 1. 2x2
 * 2. 1x1 (ocakavam chybu)
 * 3. 2x3 (ocakavam chybu)
 * 4. 3x2 (ocakavam chybu)
 * 5. 3x3 (ocakavam chybu)
 */
TEST_F(DefMatrix, SetValue_Vector){
    ASSERT_NE(m, nullptr);
//-------------------1.----------------------
    std::vector<std::vector<double>> values2x2 = {
            { 3, 4 },
            { 1, 2 }
    };
    EXPECT_TRUE(m->set(values2x2));
//-------------------2.----------------------
    std::vector<std::vector<double>> values1x1 = {
        { 1.0 }
    };
    EXPECT_FALSE(m->set(values1x1));
//-------------------3.----------------------
    std::vector<std::vector<double>> values2x3 = {
            { 3, 4, 5.4 },
            { 1, 2.3, 6 }
    };
    EXPECT_FALSE(m->set(values2x3));
//-------------------4.----------------------
    std::vector<std::vector<double>> values3x2 = {
            { 3.4, 4 },
            { 1, 2.8 },
            { 5, 6.2 }
    };
    EXPECT_FALSE(m->set(values3x2));
//-------------------5.----------------------
    std::vector<std::vector<double>> values3x3 = {
            { 3.4, 4, 8 },
            { 1, 2.8, 9 },
            { 5, 6.2, 6 }
    };
    EXPECT_FALSE(m->set(values3x3));
}

/*
 * Kontrola pristupu do matice a vratenie jej hodnot.
 * 1. Vratenie hodnot z pozicii [1][1] a [1][0]
 * 2. Vratenie hodnot 2x2 matice (neda sa ulozit, ocakavame error)
 */
TEST_F(DefMatrix, GetValue){
    ASSERT_NE(m, nullptr);
    double value;
    EXPECT_NO_THROW(value = m->get(1, 1));
    EXPECT_EQ(value, values[1][1]);
    EXPECT_NO_THROW(value = m->get(0, 1));
    EXPECT_EQ(value, values[0][1]);

    EXPECT_ANY_THROW(value = m->get(2, 2));
}

/*
 * Porovnavanie matic:
 * 1. Rozlicnych velkosti (ocakvame chybu)
 * 2. Rovnakych velkosti, inych hodnot
 * 3. Rovnakych velkosti, rovnakych hodnot
 */
TEST_F(DefMatrix, CompareMatrix){
    ASSERT_NE(m, nullptr);
//-------------------1.----------------------
    std::vector<std::vector<double>> values3x3 = {
            { 3.4, 4, 8 },
            { 1, 2.8, 9 },
            { 5, 6.2, 6 }
    };
    Matrix *mBadSize = new Matrix(3, 3);
    mBadSize->set(values3x3);
    EXPECT_ANY_THROW(*m == *mBadSize);
    delete mBadSize;
//-------------------2.----------------------
    Matrix *mDifferent = new Matrix(2, 2);
    EXPECT_FALSE(*m == *mDifferent);
    delete mDifferent;
//-------------------3.----------------------
    Matrix *mSame = new Matrix(2, 2);
    mSame->set(values);
    EXPECT_TRUE(*m == *mSame);
    delete mSame;
}

/*
 * Scitanie matic:
 * 1. Nezhodujucej sa velkosti
 * 2. Zhodujucej sa velkosti
 */
TEST_F(DefMatrix, AddMatrix){
    ASSERT_NE(m, nullptr);
//-------------------1.----------------------
    std::vector<std::vector<double>> values3x3 = {
            { 3.4, 4, 8 },
            { 1, 2.8, 9 },
            { 5, 6.2, 6 }
    };
    Matrix *mBadSize = new Matrix(3, 3);
    mBadSize->set(values3x3);
    EXPECT_ANY_THROW(*m + *mBadSize);
    delete mBadSize;
//-------------------2.----------------------
    Matrix *mSameSize = new Matrix(row, column);
    EXPECT_NO_THROW(mSameSize->set( {{ 1, 2 },
                                     { 3, 1 }}));
    Matrix *mExpectedResult = new Matrix(row, column);
    EXPECT_NO_THROW(mExpectedResult->set({ { 9.1, 3.6 },
                                           { 5.1, 3.0 } }));
    Matrix mResult;
    EXPECT_NO_THROW(mResult = *m + *mSameSize);
    EXPECT_TRUE(*mExpectedResult == mResult);
    delete mSameSize;
    delete mExpectedResult;
}

/*
 * Nasobeni matice maticou.
 * 1. Nasobenie nezhodujucimi sa maticami.
 * 2. Kontrola vypoctu
 */
TEST_F(DefMatrix, MultiplyMatrix){
    ASSERT_NE(m, nullptr);
//-------------------1.----------------------
    Matrix *m1 = new Matrix(3, 3);
    EXPECT_ANY_THROW(*m1 * *m);
    delete m1; m1 = nullptr;
    m1 = new Matrix(2, 3);
    EXPECT_ANY_THROW(*m1 * *m);
    EXPECT_NO_THROW(*m * *m1);
    delete m1; m1 = nullptr;
    m1 = new Matrix(3, 2);
    EXPECT_ANY_THROW(*m * *m1);
    EXPECT_NO_THROW(*m1 * *m);
    delete m1;
//-------------------2.----------------------
    Matrix *m2 = new Matrix(row, column);
    Matrix *expectMat = new Matrix(row, column);
    Matrix res;
    m->set({ { 2,  4 },
             { 3, -2 } });
    m2->set({ { 4, -4 },
              { 1,  0 } });
    expectMat->set({ { 12, -8 },
                     { 10, -12 } });
    EXPECT_NO_THROW(res = *m * *m2);
    EXPECT_TRUE(res == *expectMat);
    delete expectMat;
    delete m2;
}

/*
 * Nasobenie matice skalarnou hodnotou:
 * 1. Nulou
 * 2. Kladnym cislom
 * 3. Zapornym cislom
 */
TEST_F(DefMatrix, MultiplyMatrixWithValue){
    ASSERT_NE(m, nullptr);
    Matrix *expectMat = new Matrix(row, column);
    Matrix res;
//-------------------1.----------------------
    expectMat->set({ { 0, 0 }, { 0, 0 } });
    EXPECT_NO_THROW(res = *m * 0.0);
    EXPECT_TRUE(res == *expectMat);
//-------------------2.----------------------
    for (int r = 0; r < row; r++){
        for (int c = 0; c < column; c++){
            expectMat->set(r, c, 6.9 * values[r][c]);
        }
    }
    EXPECT_NO_THROW(res = *m * 6.9);
    EXPECT_TRUE(res == *expectMat);
//-------------------3.----------------------
    for (int r = 0; r < row; r++){
        for (int c = 0; c < column; c++){
            expectMat->set(r, c, -6.9 * values[r][c]);
        }
    }
    EXPECT_NO_THROW(res = *m * -6.9);
    EXPECT_TRUE(res == *expectMat);
    delete expectMat;
}

/*
 * Transpozovanie matice. Prehodime riadky za stlpce.
 */
TEST_F(DefMatrix, TransposeMatrix){
    ASSERT_NE(m, nullptr);
    Matrix *expectMat = new Matrix(column, row);
    for(int r = 0; r < column; r++){
        for(int c = 0; c < row; c++){
            EXPECT_NO_THROW(expectMat->set(r,c, values[c][r]));
        }
    }
    EXPECT_TRUE(*expectMat == m->transpose());
    delete expectMat;
}

/*** Konec souboru white_box_tests.cpp ***/
