import database
from datetime import datetime

# 用于暂存用户的申请信息
pending_applications = []


def register_user(username, password, email, role='User'):
    query = """
        INSERT INTO User (Username, Password, Email, Role) 
        VALUES (%s, %s, %s, %s)
    """
    params = (username, password, email, role)
    database.execute_query(query, params)


def login_user(username, password):
    query = """
        SELECT * 
        FROM User 
        WHERE Username=%s AND Password=%s
    """
    params = (username, password)
    result = database.fetch_data(query, params)
    return result[0] if result else None


def add_activity(title, description, location, time, max_participants, requirements):
    query = """
        INSERT INTO Activity (Title, Description, Location, Time, MaxParticipants, Requirements) 
        VALUES (%s, %s, %s, %s, %s, %s)
    """
    params = (title, description, location, time, max_participants, requirements)
    database.execute_query(query, params)


def update_activity(activity_id, title, description, location, time, max_participants, requirements):
    query = """
        UPDATE Activity 
        SET Title=%s, Description=%s, Location=%s, Time=%s, MaxParticipants=%s, Requirements=%s
        WHERE ActivityID=%s
    """
    params = (title, description, location, time, max_participants, requirements, activity_id)
    database.execute_query(query, params)


def delete_activity(activity_id):
    query = """
        DELETE FROM Activity 
        WHERE ActivityID=%s
    """
    params = (activity_id,)
    database.execute_query(query, params)


def apply_to_activity(user_id, activity_id):
    # 检查活动是否存在
    activity_query = """
        SELECT * 
        FROM Activity 
        WHERE ActivityID=%s
    """
    params = (activity_id,)
    activity = database.fetch_data(activity_query, params)

    if not activity:
        print(f"Activity with ID {activity_id} does not exist.")
        return

    # 插入申请记录
    application_query = """
        INSERT INTO Application (UserID, ActivityID, Status, ApplyTime) 
        VALUES (%s, %s, %s, NOW())
    """
    application_params = (user_id, activity_id, 'Pending')
    database.execute_query(application_query, application_params)
    print(f"Application saved: UserID={user_id}, ActivityID={activity_id}, Status=Pending")

    # 暂存用户的申请信息
    pending_applications.append((activity_id, 1))
    print(f"Application added to pending list: UserID={user_id}, ActivityID={activity_id}")


def update_pending_applications():
    if not pending_applications:
        return

    # 批量更新活动的参与者数量
    for activity_id, increment in pending_applications:
        update_activity_query = """
            UPDATE Activity 
            SET CurrentParticipants = CurrentParticipants + %s 
            WHERE ActivityID=%s
        """
        update_params = (increment, activity_id)
        database.execute_query(update_activity_query, update_params)
        print(f"Updated participants count for ActivityID={activity_id}")

    # 清空暂存列表
    pending_applications.clear()


def get_application_status(application_id):
    query = """
        SELECT Status 
        FROM Application 
        WHERE ApplicationID=%s
    """
    params = (application_id,)
    result = database.fetch_data(query, params)
    return result[0]['Status'] if result else None


def get_activity_applicants(activity_id):
    query = """
        SELECT u.Username, a.ApplyTime, a.Status, a.ApplicationID
        FROM User u
        JOIN Application a ON u.UserID = a.UserID
        WHERE a.ActivityID = %s
    """
    params = (activity_id,)
    applicants = database.fetch_data(query, params)
    print(f"Applicants for activity {activity_id}: {applicants}")
    return applicants


def update_application_status(application_id, status):
    query = """
        UPDATE Application 
        SET Status=%s 
        WHERE ApplicationID=%s
    """
    params = (status, application_id)
    database.execute_query(query, params)
    print(f"Application status updated: ApplicationID={application_id}, Status={status}")


def get_activities():
    query = """
        SELECT * 
        FROM Activity
    """
    return database.fetch_data(query)