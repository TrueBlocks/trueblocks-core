import vcr
from pytest import fixture
from chifra import chifra

@fixture
def tv_keys():
    # Responsible only for returning the test data
    return ['blockNumber', 'timestamp']

@vcr.use_cassette('tests/data/block.yml')
def test_tv_info(tv_keys):
    """Tests an API call to get a TV show's info"""

    chifra = chifra(1396)
    response = chifra.info()

    assert isinstance(response, dict)
    assert response['id'] == 1396, "The ID should be in the response"
    assert set(tv_keys).issubset(response.keys()), "All keys should be in the response"

@vcr.use_cassette('tests/data/when.yml')
def test_tv_popular(tv_keys):
    """Tests an API call to get a popular tv shows"""

    response = chifra.blocks()

    assert isinstance(response, dict)
    assert isinstance(response['data'], list)
    assert isinstance(response['data'][0], dict)
    assert set(tv_keys).issubset(response['data'][0].keys())
