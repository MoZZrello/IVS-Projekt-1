//======== Copyright (c) 2022, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Richard Harman <xharma05@stud.fit.vutbr.cz>
// $Date:       $2022-02-17
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Richard Harman
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

class NonEmptyTree : public ::testing::Test{
protected:
    BinaryTree *bt;
    const static int length = 15;
    int values[length] = { 5, 8, 10, 15, 20, 30, 40, 50, 55, 60, 69, 70, 80, 85, 90};
    virtual void SetUp(){
        bt = new BinaryTree();
        for(auto &value : values) {
            bt->InsertNode(value);
        }
    }
    virtual void TearDown(){
        delete bt;
    }
};

class EmptyTree : public ::testing::Test{
protected:
    BinaryTree *bt;
    virtual void SetUp(){
        bt = new BinaryTree();
    }
    virtual void TearDown(){
        delete bt;
    }
};

class TreeAxioms : public ::testing::Test{
protected:
    BinaryTree *bt;
    virtual void SetUp(){
        bt = new BinaryTree();
        bt->InsertNode(3);
        bt->InsertNode(1);
        bt->InsertNode(5);
    }
    virtual void TearDown(){
        delete bt;
    }
};

//-----------------EMPTY TREES-----------------

/*
 * Prida par uzlov do prazdneho stromu v hodnotach <0;20>
 */
TEST_F(EmptyTree, InsertNode){
    std::pair<bool, BinaryTree::Node_t *> temp;

    for (int i = 0; i < 20; i++)
    {
        EXPECT_NO_THROW(temp = bt->InsertNode(i));
        ASSERT_TRUE(temp.first);
        ASSERT_FALSE(temp.second == NULL);
    }
    EXPECT_FALSE(bt->GetRoot() == NULL);
}

/*
 * Odstrani par uzlov z prazdneho stromu v hodnotach <0;20>.
 */
TEST_F(EmptyTree, DeleteNode){
    for (int i = 0; i <= 20; i++){
        EXPECT_FALSE(bt->DeleteNode(i));
    }
}

/*
 * Vyhlada par uzlov v prazdnom strome v hodnotach <0;20>.
 */
TEST_F(EmptyTree, FindNode){
    for (int i = 0; i <= 20; i++){
        EXPECT_TRUE(bt->FindNode(i) == NULL);
    }
}
//----------------NONEMPTY TREES-----------------

/*
 * Prida uzly do neprazdneho stromu. Ak dany uzol uz existuje, vrati false.
 * Tvorime uzly na intervale 0 az 100.
 */
TEST_F(NonEmptyTree, InsertNode){
    int index = 0;
    std::pair<bool, BinaryTree::Node_t *> temp;
    for (int i = 0; i <= 100; i++){
        EXPECT_NO_THROW(temp = bt->InsertNode(i));
        ASSERT_TRUE(temp.second != NULL);
        if (index < length){
            if (i == values[index]){
                EXPECT_FALSE(temp.first);
                index++;
            }else{
                EXPECT_TRUE(temp.first);
            }
        }else{
            EXPECT_TRUE(temp.first);
        }
        EXPECT_EQ(temp.second->key, i);
    }
}

/*
 * 1. Test najprv skusi odstranit uzly ktore v strome niesu s ocakavanim vratenia bool hodnoty False.
 * 2. Nasledne odstranime uzly ktore v strome sú.
 * 3. Skúsime odstranit uzly, ktore sme prave odstranili.
 * 4. Zmazem koren. Zvysok stromu by mal stale existovat. Koren by sa mal nahradit.
 */
TEST_F(NonEmptyTree, DeleteNode_Random){
//-------------------1.----------------------
    EXPECT_FALSE(bt->DeleteNode(0));
    EXPECT_FALSE(bt->DeleteNode(100));
//-------------------2.----------------------
    EXPECT_TRUE(bt->DeleteNode(10));
    EXPECT_TRUE(bt->DeleteNode(55));
    EXPECT_TRUE(bt->DeleteNode(90));
//-------------------3.----------------------
    EXPECT_FALSE(bt->DeleteNode(10));
    EXPECT_FALSE(bt->DeleteNode(55));
//-------------------4.----------------------
    Node_t *root;
    EXPECT_NO_THROW(root = bt->GetRoot());
    int key = root->key;
    EXPECT_TRUE(bt->DeleteNode(root->key));
    EXPECT_TRUE(bt->GetRoot() != NULL);
    EXPECT_NE(bt->GetRoot()->key, key);
}

/*
 * 1. Odstrani vsetky uzly v celom strome.
 * 2. Pokusi sa vyhladat odstranene uzly.
 */
TEST_F(NonEmptyTree, DeleteNode_All){
//-------------------1.----------------------
    for (int i = 0; i < length; i++){
        EXPECT_TRUE(bt->DeleteNode(values[i]));
    }
//-------------------2.----------------------
    for (int i = 0; i < length; i++){
        EXPECT_TRUE(bt->FindNode(values[i]) == NULL);
    }
    EXPECT_TRUE(bt->GetRoot() == NULL);
}

/*
 * Prehliada vsetky uzly v strome na intervale <0;100>.
 * Pole musi nyt zoradene.
 */
TEST_F(NonEmptyTree, FindNode){
    int index = 0;
    for (int i = 0; i <= 100; i++){
        if (index < length){
            if (i == values[index]){
                ASSERT_TRUE(bt->FindNode(i));
                index++;
             }else
                ASSERT_FALSE(bt->FindNode(i));
        }else{
            ASSERT_FALSE(bt->FindNode(i));
        }
    }
}

//------------------AXIOMS-------------------

/*
 * Axiom: Vsechny listove uzly (tedy uzly bez potomku) jsou cerne.
 * 1. Vyhlada vsetky listove uzly pomocou funkcie GetLeafNodes().
 * 2. Prejde vsetkymi listami, kontroluje ci sa skutocne jedna o list cez ASSERT_TRUE.
 * 3. Zisti ci ma list ciernu farbu
 */
TEST_F(TreeAxioms, Axiom1){
//-------------------1.----------------------
    std::vector<BinaryTree::Node_t *> leafNodes;
    EXPECT_NO_THROW(bt->GetLeafNodes(leafNodes));
//-------------------2.----------------------
    for (const Node_t *leaf : leafNodes){
        ASSERT_TRUE(leaf->pRight == NULL);
        ASSERT_TRUE(leaf->pLeft == NULL);
//-------------------3.----------------------
        EXPECT_EQ(leaf->color, Color_t::BLACK);
    }
}

/*
 * Axiom: Pokud je uzel cerveny, pak jsou jeho oba potomci cerne.
 * 1. Nacitam si vsetky uzly
 * 2. Prejdem vsetky uzly a ak sa parent node nerovna NULL tak kontrolujem jeho farbu a farbu jeho synov.
 */
TEST_F(TreeAxioms, Axiom2){
//-------------------1.----------------------
    std::vector<BinaryTree::Node_t *> allNodes;
    EXPECT_NO_THROW(bt->GetAllNodes(allNodes));
//-------------------2.----------------------
    for(const Node_t *node : allNodes){
        if(node->color == Color_t::RED){
           if(node->pParent != NULL){
                EXPECT_NE(node->pParent->color, Color_t::RED);
                EXPECT_EQ(node->pLeft->color, Color_t::BLACK);
                EXPECT_EQ(node->pRight->color, Color_t::BLACK);
           }
        }
    }
}

/*
 * Axiom: Kazda cesta od kazdeho listoveho uzlu ke koreni obsahuje stejny pocet cernych uzlu.
 * 1. Nahram si vsetky listy
 * 2. Overim ze su listy
 * 3. Pri prvom liste si nacitam pocet ciernych  ciest ku korenu
 * 4. Vsetky dalsie listy nacitam znova pocet ciernych ku korenu a porovnam to s prvym (ak sa nezhoduje, tak FALSE)
 */
TEST_F(TreeAxioms, Axiom3){
//-------------------1.----------------------
    std::vector<BinaryTree::Node_t *> leafNodes;
    int blackCount = 0;
    EXPECT_NO_THROW(bt->GetLeafNodes(leafNodes));
//-------------------2.----------------------
    for(Node_t *leaf : leafNodes){
        ASSERT_TRUE(leaf->pRight == NULL);
        ASSERT_TRUE(leaf->pLeft == NULL);
        EXPECT_EQ(leaf->color, Color_t::BLACK);
//-------------------3.----------------------
        if(blackCount == 0){
            for(Node_t *temp = leaf; temp != NULL; temp = temp->pParent){
                if (temp->color == Color_t::BLACK){
                        blackCount++;
                }
            }
//-------------------4.----------------------
        }else{
            int blackCompareCount = 0;
            for(Node_t *temp = leaf; temp != NULL; temp = temp->pParent){
                if (temp->color == Color_t::BLACK){
                    blackCompareCount++;
                }
            }
            EXPECT_EQ(blackCount,blackCompareCount);
        }
    }
}

/*** Konec souboru black_box_tests.cpp ***/
