#include "SMSong.h"
#include "minunit.h"

SMSong* song;

MU_TEST(create){

    /* act */
    song = SMSong_load("res/Songs/iturntoyou/iturntoyou.sm");

    /* assert */
    mu_check(song != NULL);

    mu_assert_str_eq("I Turn to You", song->title);
    mu_assert_str_eq("Hex Hector Vocal Mix", song->subtitle);
    mu_assert_str_eq("Melanie C", song->artist);
    mu_assert_str_eq("", song->titletranslit);
    mu_assert_str_eq("", song->subtitletranslit);
    mu_assert_str_eq("", song->artisttranslit);
    mu_assert_str_eq("Plaguefox (gamecast.org/plaguemix)", song->credit);
    mu_assert_str_eq("iturntoyou-bn.png", song->banner);
    mu_assert_str_eq("iturntoyou-bg.jpg", song->background);
    mu_assert_str_eq("", song->lyricspath);
    mu_assert_str_eq("pm1cd.png", song->cdtitle);
    mu_assert_str_eq("iturntoyou.ogg", song->music);
    mu_assert_int_eq(-3, song->offset_in_ms);
    mu_assert_int_eq(53897, song->samplestart_in_ms);
    mu_assert_int_eq(15647, song->samplelength_in_ms);
    mu_check(song->BPMs);
    mu_assert_int_eq(0, song->BPMs[0]);
    mu_assert_int_eq(138041, song->BPMs[1]);
    mu_check(song->BGchanges != NULL);
    mu_check(song->measures != NULL);
    mu_check(song->measures[4] != NULL);
    mu_assert_int_eq('1', song->measures[4]->rows[3][0]);
    mu_assert_int_eq('0', song->measures[4]->rows[3][1]);
    mu_assert_int_eq('0', song->measures[4]->rows[3][2]);
    mu_assert_int_eq('0', song->measures[4]->rows[3][3]);
}

MU_TEST_SUITE(SMSong_suite){
    MU_RUN_TEST(create);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(SMSong_suite);
    MU_REPORT();
    return minunit_fail;
}
