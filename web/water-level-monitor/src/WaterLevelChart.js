import React, { useState, useEffect } from 'react';

const WaterLevelTable = ({ userEmail, refreshTrigger }) => {
  const [data, setData] = useState([]);
  const [error, setError] = useState(null);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    const fetchData = async () => {
      try {
        if (data.length === 0 && !error) {
          setLoading(true);
        }
        setError(null);

        // get the user's ID based on email
        const userResponse = await fetch(`/api/users?email=${userEmail}`);
        if (!userResponse.ok) {
          throw new Error('User not found or unable to retrieve user ID.');
        }
        const userData = await userResponse.json();
        const userId = (userData && Array.isArray(userData) && userData.length > 0) ? userData[0].id : null;
        const waterSenseUrls = (userData && Array.isArray(userData) && userData.length > 0) ? userData[0].waterSenses : [];

        if (!userId) {
          throw new Error('User ID not found for the provided email or no user data returned.');
        }

        if (waterSenseUrls.length === 0) {
          setData([]);
          setLoading(false);
          return;
        }

        // fetch water level data for each specific water sense URL
        const waterLevelPromises = waterSenseUrls.map(url => fetch(url).then(res => {
          if (!res.ok) {
            throw new Error(`Failed to fetch water level data from ${url}.`);
          }
          return res.json();
        }));

        const waterLevelResults = await Promise.all(waterLevelPromises);
        
        setData(waterLevelResults);
      } catch (err) {
        console.error("Error fetching water level data:", err);
        setError(err.message);
        setData([]);
      } finally {
        setLoading(false);
      }
    };

    fetchData();
  }, [userEmail, refreshTrigger]);

  if (loading) {
    return <div className="table-container">Loading water level data...</div>;
  }

  if (error) {
    return <div className="table-container error-message">Error: {error}</div>;
  }

  return (
    <div className="table-container">
      <h1>Water Level Data</h1>
      {data.length === 0 ? (
        <p>No water level data available for this user.</p>
      ) : (
        <table>
          <thead>
            <tr>
              <th>ID</th>
              <th>Water Level</th>
            </tr>
          </thead>
          <tbody>
            {data.map((item) => (
              <tr key={item.id}>
                <td>{item.id}</td>
                <td>{item.water_level}</td>
              </tr>
            ))}
          </tbody>
        </table>
      )}
    </div>
  );
};

export default WaterLevelTable;
